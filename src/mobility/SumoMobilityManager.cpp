#include "mobility/SumoMobilityManager.hpp"
#include "traci/PosixLauncher.h"
#include "core/Config.hpp"
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
    mLauncher.reset(new traci::PosixLauncher(getGlobalConfig()["launcher"]));

    mTraci->connect(mLauncher->launch());
    mUpdater.reset(new SumoUpdater(*mLite));
    mTraci->simulationStep(mUpdateInterval.count()/1000);
}

std::shared_ptr<MobilityManagerData> SumoMobilityManager::doVehicleUpdate(std::shared_ptr<Action> action, ObjectContainer_ptr objectList)
{
    auto t = action->getStartTime();
    auto res = mUpdater->step(std::chrono::duration_cast<std::chrono::milliseconds>(t));
    auto data = executeUpdate(res, objectList);

    if(mLite->simulation().getMinExpectedNumber() > 0) {
        auto newAction = createSelfAction(std::chrono::milliseconds(50), action->getStartTime() + std::chrono::milliseconds(100));
        data->actionsToSchedule.push_back(newAction);
    }

    DLOG_F(INFO, "SUMO Update");
    mTraci->simulationStep(mUpdateInterval.count()/1000);
    return std::move(data);
}

std::shared_ptr<MobilityManagerData> SumoMobilityManager::executeUpdate(const SumoUpdater::Results& r, ObjectContainer_ptr objectContainer)
{
    // add vehicles

    std::shared_ptr<MobilityManagerData> data = std::make_shared<MobilityManagerData>();
    for(auto& vehicle : r.departedVehicles) {
        auto vehicleObject = ObjectFactory::getInstance().createObject("vehicle", objectContainer);
        data->vehiclesToAdd.push_back(vehicleObject);
    }



    // remove vehicles

    // update vehicles

    return std::move(data);
}

}
