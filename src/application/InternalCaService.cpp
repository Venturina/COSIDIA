#include "application/InternalCaService.hpp"
#include "objects/VehicleObject.hpp"
#include "networking/Router.hpp"
#include "networking/Runtime.hpp"

#include <vanetza/btp/data_request.hpp>
#include <vanetza/geonet/data_confirm.hpp>
#include <vanetza/geonet/router.hpp>
#include <boost/units/systems/si/prefixes.hpp>
#include <boost/units/systems/angle/degrees.hpp>

namespace cosidia
{

InternalCaService::InternalCaService(Router* router) : mRouter(router)
{
    enforce(router, "InternalCaService: invalid router");
}

bool InternalCaService::triggerCam(const vanetza::PositionFix* fix, Action* action, vanetza::geonet::Router* router, vanetza::geonet::MIB& mib) {
    auto cam = makeCam(fix, action);

    vanetza::geonet::Router::DownPacketPtr packet { new vanetza::geonet::DownPacket() };
    packet->layer(vanetza::OsiLayer::Application) = std::move(cam);

    vanetza::btp::DataRequestGeoNetParams request;
    request.its_aid = vanetza::aid::CA;
    request.transport_type = vanetza::geonet::TransportType::SHB;
    request.communication_profile = vanetza::geonet::CommunicationProfile::ITS_G5;

    vanetza::geonet::GbcDataRequest gbcRequest(mib);
    gbcRequest.upper_protocol = vanetza::geonet::UpperProtocol::BTP_B;
    gbcRequest.communication_profile = request.communication_profile;
    gbcRequest.its_aid = request.its_aid;
    if(request.maximum_lifetime) {
        gbcRequest.maximum_lifetime = request.maximum_lifetime.get();
    }
    gbcRequest.repetition = request.repetition;
    gbcRequest.traffic_class = request.traffic_class;

    auto confirm = router->request(gbcRequest, std::make_unique<vanetza::DownPacket>(*packet));
    enforce(confirm.accepted(), "Router: Request was not accepted");
    return confirm.accepted();
}

vanetza::asn1::Cam InternalCaService::makeCam(const vanetza::PositionFix* fix, Action* action)
{
    vanetza::asn1::Cam message;
    ItsPduHeader_t& header = message->header;
    header.protocolVersion = 2;
    header.messageID = ItsPduHeader__messageID_cam;
    header.stationID = 1; // some dummy value

    CoopAwareness_t& cam = message->cam;


    auto microseconds = Runtime::convert(action->getStartTime()).time_since_epoch(); //TODO switch to UTC
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(microseconds).count();

    cam.generationDeltaTime = milliseconds % 65536;

    auto longitude = fix->longitude * (1000 * 1000 * boost::units::degree::degrees) * boost::units::si::micro; // TODO Ask raphael
    auto latitude = fix->latitude * (1000 * 1000 * boost::units::degree::degrees) * boost::units::si::micro;

    BasicContainer_t& basic = cam.camParameters.basicContainer;
    basic.stationType = StationType_passengerCar;

    auto& refPos = basic.referencePosition;
    refPos.longitude = std::round(longitude.value()) * Longitude_oneMicrodegreeEast;
    refPos.latitude = std::round(latitude.value()) * Latitude_oneMicrodegreeNorth;
    refPos.positionConfidenceEllipse.semiMajorOrientation = HeadingValue_unavailable;
    refPos.positionConfidenceEllipse.semiMajorConfidence = SemiAxisLength_unavailable;
    refPos.positionConfidenceEllipse.semiMinorConfidence = SemiAxisLength_unavailable;
    refPos.altitude.altitudeConfidence = AltitudeConfidence_unavailable;
    refPos.altitude.altitudeValue = AltitudeValue_unavailable;

    cam.camParameters.highFrequencyContainer.present = HighFrequencyContainer_PR_basicVehicleContainerHighFrequency;

    BasicVehicleContainerHighFrequency& bvc = cam.camParameters.highFrequencyContainer.choice.basicVehicleContainerHighFrequency;
    std::cout << fix->course.value().value() * 10 << std::endl;
    //bvc.heading.headingValue = HeadingValue_unavailable;
    bvc.heading.headingValue = fix->course.value().value() * 10;
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

    std::cout << "Generated CAM from " << mRouter->getObjectId().raw() << std::endl;

    return message;
}

} // namespace cosidia