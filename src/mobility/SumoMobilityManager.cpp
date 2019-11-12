#include "mobility/SumoMobilityManager.hpp"
#include "traci/PosixLauncher.h"
#include "core/Core.hpp"

#include "loguru/loguru.hpp"

namespace paresis
{
void SumoMobilityManager::initObject(std::shared_ptr<Action> action)
{
    mUpdateInterval = std::chrono::milliseconds(100);
    auto nextAction = std::make_shared<Action>(std::chrono::milliseconds(10),
        Action::Kind::START, action->getStartTime()+mUpdateInterval, mObjectId);

    getCoreP()->scheduleAction(nextAction);
    mTraci.reset(new traci::API());
    mLite.reset(new traci::LiteAPI(*mTraci));
    mLauncher.reset(new traci::PosixLauncher());

    mTraci->connect(mLauncher->launch());
}

std::shared_ptr<MobilityManagerData> SumoMobilityManager::doVehicleUpdate(std::shared_ptr<Action> action, ObjectContainer_ptr objectList)
{
    mTraci->simulationStep(mUpdateInterval.count()/1000);
    auto newAction = std::make_shared<Action>(std::chrono::milliseconds(10),
            Action::Kind::START, action->getStartTime() + std::chrono::milliseconds(100),
            mObjectId);

    DLOG_F(INFO, "SUMO Update");

    auto data = std::make_shared<MobilityManagerData>();
    data->actionsToSchedule.push_back(newAction);

    return data;
}

}