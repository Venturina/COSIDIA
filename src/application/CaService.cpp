#include "application/CaService.hpp"
#include "application/ServiceActionData.hpp"
#include "core/Core.hpp"
#include "networking/Runtime.hpp"

#include <vanetza/btp/data_request.hpp>
#include <vanetza/btp/ports.hpp>
#include <vanetza/facilities/cam_functions.hpp>
#include <vanetza/geonet/router.hpp>
#include <vanetza/units/angle.hpp>
#include <chrono>
#include <boost/units/systems/si/prefixes.hpp>
#include <boost/units/systems/angle/degrees.hpp>

namespace cosidia
{

CaService::CaService() : BaseObject()
{
    mObjectName = "CaService";
}

void CaService::initObject(std::shared_ptr<Action> action)
{
    auto newAction = createSelfAction(std::chrono::milliseconds(2), std::chrono::milliseconds(100) + action->getStartTime());
    newAction->setType("CaInit"_sym);

    getCoreP()->scheduleAction(newAction);
}

void CaService::startExecution(std::shared_ptr<Action> action)
{
    if(action->getType() == "CaUpdate"_sym) {
        boost::fibers::packaged_task<CaServiceUpdateData(std::shared_ptr<Action>, std::shared_ptr<const VehicleObjectContext>)>
            pt (std::bind(&CaService::executeUpdate, this, std::placeholders::_1, std::placeholders::_2));

        mFuture = pt.get_future();
        boost::fibers::fiber(std::move(pt), action, mVehicleObject.lock()->getContext()).detach();
    } else if(action->getType() == "CaIndication"_sym) {

    } else if(action->getType() == "CaInit"_sym) {
        boost::fibers::packaged_task<CaServiceUpdateData(std::shared_ptr<Action>, ConstObjectContainer_ptr)>
            pt (std::bind(&CaService::init, this, std::placeholders::_1, std::placeholders::_2));

        mFuture = pt.get_future();
        boost::fibers::fiber(std::move(pt), action, getCoreP()->getCurrentObjectList()).detach();
    } else {
        std::cout << action->getType().value() << std::endl;
        throw std::runtime_error("CaService: wrong BeginAction received");
    }
}

CaServiceUpdateData CaService::init(std::shared_ptr<Action> action, ConstObjectContainer_ptr objList) {
    auto router = getParentByName(this, "Router", objList);
    auto mobility = getSiblingByName(router.lock()->getObjectId(), "VehicleObject", objList);
    mRouterId = router.lock()->getObjectId();

    mVehicleObject = std::static_pointer_cast<VehicleObject>(mobility.lock());

    auto newAction = createSelfAction(std::chrono::milliseconds(2), std::chrono::milliseconds(100) + action->getStartTime());
    newAction->setType("CaUpdate"_sym);

    CaServiceUpdateData data;
    data.actionsToSchedule.push_back(newAction);
    return data;
}

void CaService::endExecution(std::shared_ptr<Action> action)
{
    if(action->getType() == "CaUpdate"_sym) {
        auto data = mFuture.get();
        for(auto action : data.actionsToSchedule) {
            getCoreP()->scheduleAction(std::move(action));
        }
        if(data.actionToDelete) {
            getCoreP()->removeAction(data.actionToDelete);
        }
    } else if( action->getType() == "CaIndication"_sym) {
        auto data = mFuture.get();
    }else if(action->getType() == "CaInit"_sym) {
        auto data = mFuture.get();
        for(auto action : data.actionsToSchedule) {
            getCoreP()->scheduleAction(std::move(action));
        }
    } else {
        std::cout << action->getType().value() << std::endl;
        throw std::runtime_error("CaService: wrong EndAction received");
    }
}

CaServiceUpdateData CaService::executeUpdate(std::shared_ptr<Action> action, std::shared_ptr<const VehicleObjectContext> context)
{
    vanetza::asn1::Cam message;
    makeCam(message, action.get(), context.get());

    vanetza::geonet::Router::DownPacketPtr packet { new vanetza::geonet::DownPacket() };
    packet->layer(vanetza::OsiLayer::Application) = std::move(message);

    vanetza::btp::DataRequestGeoNetParams request;
    request.its_aid = vanetza::aid::CA;
    request.transport_type = vanetza::geonet::TransportType::SHB;
    request.communication_profile = vanetza::geonet::CommunicationProfile::ITS_G5;

    auto serviceActionData = std::make_shared<ServiceActionData>();
    serviceActionData->packet = std::move(packet);
    serviceActionData->request = request;
    serviceActionData->port = vanetza::btp::ports::CAM;

    auto transmitAction = std::make_shared<Action>(std::chrono::milliseconds(1), Action::Kind::START, action->getStartTime() + action->getDuration() 
    + std::chrono::milliseconds(1), mRouterId, mObjectId);

    transmitAction->setType("ServiceRequest"_sym);
    transmitAction->setActionData(serviceActionData);

    auto newAction = createSelfAction(std::chrono::milliseconds(2), std::chrono::milliseconds(100) + action->getStartTime());
    newAction->setType("CaUpdate"_sym);

    CaServiceUpdateData data;
    data.actionsToSchedule.push_back(transmitAction);
    data.actionsToSchedule.push_back(newAction);
    return data;
}


void CaService::makeCam(vanetza::asn1::Cam& message, Action* action, const VehicleObjectContext* context) {
    ItsPduHeader_t& header = message->header;
    header.protocolVersion = 2;
    header.messageID = ItsPduHeader__messageID_cam;
    header.stationID = 1; // some dummy value

    CoopAwareness_t& cam = message->cam;


    auto microseconds = Runtime::convert(action->getStartTime()).time_since_epoch(); //TODO switch to UTC
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(microseconds).count();

    cam.generationDeltaTime = milliseconds;

    auto longitude = context->geo.longitude * 1000 * 1000 * boost::units::degree::degrees * boost::units::si::micro; // TODO Ask raphael
    auto latitude = context->geo.latitude * 1000 * 1000 * boost::units::degree::degrees * boost::units::si::micro;

    BasicContainer_t& basic = cam.camParameters.basicContainer;
    basic.stationType = StationType_passengerCar;

    auto& refPos = basic.referencePosition;
    refPos.longitude = std::round(longitude.value()) * Longitude_oneMicrodegreeEast;
    refPos.latitude = std::round(latitude.value()) * Latitude_oneMicrodegreeNorth;
    refPos.positionConfidenceEllipse.semiMajorOrientation = context->heading; //TODO check heading
    refPos.positionConfidenceEllipse.semiMajorConfidence = SemiAxisLength_unavailable;
    refPos.positionConfidenceEllipse.semiMinorConfidence = SemiAxisLength_unavailable;
    refPos.altitude.altitudeConfidence = AltitudeConfidence_unavailable;
    refPos.altitude.altitudeValue = AltitudeValue_unavailable;

    cam.camParameters.highFrequencyContainer.present = HighFrequencyContainer_PR_basicVehicleContainerHighFrequency;

    BasicVehicleContainerHighFrequency& bvc = cam.camParameters.highFrequencyContainer.choice.basicVehicleContainerHighFrequency;
    bvc.heading.headingValue = 0;
    bvc.heading.headingConfidence = HeadingConfidence_equalOrWithinOneDegree;

    bvc.speed.speedValue = 0;
    bvc.speed.speedConfidence = SpeedConfidence_equalOrWithinOneCentimeterPerSec;

    bvc.driveDirection = DriveDirection_forward;
    bvc.longitudinalAcceleration.longitudinalAccelerationValue = LongitudinalAccelerationValue_unavailable;

    bvc.vehicleLength.vehicleLengthValue = VehicleLengthValue_unavailable;
    bvc.vehicleLength.vehicleLengthConfidenceIndication = VehicleLengthConfidenceIndication_noTrailerPresent;
    bvc.vehicleWidth = VehicleWidth_unavailable;

    bvc.curvature.curvatureValue = 0;
    bvc.curvature.curvatureConfidence = CurvatureConfidence_unavailable;
    bvc.curvatureCalculationMode = CurvatureCalculationMode_yawRateUsed;

    bvc.yawRate.yawRateValue = YawRateValue_unavailable;
    std::string error;
    if (!message.validate(error)) {
        throw std::runtime_error("Invalid high frequency CAM: %s" + error);
    }

    std::cout << "Generated CAM contains\n";
    vanetza::facilities::print_indented(std::cout, message, "  ", 1);
}

} // namespace cosidia
