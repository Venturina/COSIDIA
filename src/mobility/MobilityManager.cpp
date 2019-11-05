#include "core/Core.hpp"
#include "mobility/MobilityManager.hpp"
#include <boost/fiber/future.hpp>
#include <loguru/loguru.hpp>


namespace paresis
{

MobilityManager::MobilityManager() : BaseObject()
{
    mObjectName = "MobilityManager";
}

void MobilityManager::startExecution(std::shared_ptr<Action> action)
{
    DLOG_F(INFO, "MobilityManager update");
    auto objectList = getCoreP()->getCurrentObjectList();

    boost::fibers::packaged_task<std::shared_ptr<MobilityManagerData>
        (std::shared_ptr<Action>, ObjectContainer_ptr)> pt
        (std::bind(&MobilityManager::doVehicleUpdate, this, std::placeholders::_1, std::placeholders::_2));

    mFuture = pt.get_future();
    boost::fibers::fiber(std::move(pt), action, objectList).detach();
}

std::shared_ptr<MobilityManagerData> MobilityManager::doVehicleUpdate(std::shared_ptr<Action> action, ObjectContainer_ptr objectList)
{
    auto data = std::make_shared<MobilityManagerData>();
    DLOG_F(INFO, "MobilityManager: in fiber");

    if(updateCount % 10 == 0 && updateCount < 100) {
        auto vehicle = ObjectFactory::getInstance().createObject("vehicle", objectList);
        data->vehiclesToAdd.push_back(vehicle);
    }

    if(action->getStartTime() < std::chrono::seconds(20)) {
        auto newAction = std::make_shared<Action>(std::chrono::milliseconds(50),
            Action::Kind::START, action->getStartTime() + std::chrono::milliseconds(100),
            mObjectId); // ugly as hell?

        data->actionsToSchedule.push_back(newAction);
    }

    updateCount++;
    return data;
}

void MobilityManager::endExecution(std::shared_ptr<Action> action)
{
    auto data = mFuture.get();
    for (auto& vehicle : data->vehiclesToAdd) {
        DLOG_F(ERROR, "Add vehicle");
        vehicle.resolveAndStart(action->getStartTime());
    }
    for(auto& a : data->actionsToSchedule) {
        getCoreP()->scheduleAction(std::move(a));
    }
}

void MobilityManager::initObject(std::shared_ptr<Action> action)
{
    DLOG_F(INFO, "Id of MobilityManager: %d", mObjectId);

    auto newAction = std::make_shared<Action>(std::chrono::milliseconds(50),
        Action::Kind::START, action->getStartTime() + std::chrono::milliseconds(100),
        mObjectId); // ugly as hell?

    getCoreP()->scheduleAction(newAction);

}

} // namespace paresis
