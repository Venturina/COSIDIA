#include "core/Core.hpp"
#include "mobility/MobilityManager.hpp"
#include <boost/fiber/future.hpp>
#include <loguru/loguru.hpp>


namespace paresis
{

MobilityManager::MobilityManager(Core* c) : BaseObject(c), mFactory(c)
{
    mObjectName = "MobilityManager";
}

void MobilityManager::startExecution(std::shared_ptr<Action> action)
{
    DLOG_F(INFO, "MobilityManager update");
    boost::fibers::packaged_task<std::shared_ptr<MobilityManagerData>(std::shared_ptr<Action>)> pt(std::bind(&MobilityManager::doVehicleUpdate, this, std::placeholders::_1));
    mFuture = pt.get_future();
    boost::fibers::fiber(std::move(pt),action).detach();
}

std::shared_ptr<MobilityManagerData> MobilityManager::doVehicleUpdate(std::shared_ptr<Action> action)
{
    auto data = std::make_shared<MobilityManagerData>();
    DLOG_F(INFO, "MobilityManager: in fiber");

    if(updateCount % 10 == 0 && updateCount < 100) {
        data->objectsToAdd = mFactory.createObject("vehicle");
        for(auto& obj : data->objectsToAdd) {
            auto newAction = std::make_shared<Action>(std::chrono::milliseconds(0),
                Action::Kind::INIT, action->getStartTime() + action->getDuration() + std::chrono::milliseconds(10),
                obj->getObjectId());
            data->actionsToSchedule.push_back(std::move(newAction));
        }
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
    for(auto& obj : data->objectsToAdd) {
        mCore->addObject(std::move(obj));
    }
    for(auto& a : data->actionsToSchedule) {
        mCore->scheduleAction(std::move(a));
    }
}

void MobilityManager::initObject(std::shared_ptr<Action> action)
{
    DLOG_F(INFO, "Id of MobilityManager: %d", mObjectId);

    auto newAction = std::make_shared<Action>(std::chrono::milliseconds(50),
        Action::Kind::START, action->getStartTime() + std::chrono::milliseconds(100),
        mObjectId); // ugly as hell?

    mCore->scheduleAction(newAction);

}

} // namespace paresis
