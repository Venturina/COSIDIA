#include "mobility/SumoMobilityManager.hpp"
#include "objects/VehicleObject.hpp"
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

std::shared_ptr<MobilityManagerTasks> SumoMobilityManager::doVehicleUpdate(std::shared_ptr<Action> action, ConstObjectContainer_ptr objectList)
{
    fetchVehicleIds(objectList);

    auto t = action->getStartTime();
    auto res = mUpdater->step(std::chrono::duration_cast<std::chrono::milliseconds>(t));
    auto data = executeUpdate(res, objectList, action);

    if(mLite->simulation().getMinExpectedNumber() > 0) {
        auto newAction = createSelfAction(std::chrono::milliseconds(50), action->getStartTime() + std::chrono::milliseconds(100));
        data->actionsToSchedule.push_back(newAction);
    }

    DLOG_F(INFO, "SUMO Update");
    mTraci->simulationStep(mUpdateInterval.count()/1000);
    return std::move(data);
}

std::shared_ptr<MobilityManagerTasks> SumoMobilityManager::executeUpdate(const SumoUpdater::Results& updaterResult, ConstObjectContainer_ptr objectContainer, std::shared_ptr<Action> action)
{
    // add vehicles
    std::shared_ptr<MobilityManagerTasks> coreTasks = std::make_shared<MobilityManagerTasks>();
    for(auto& vehicle : updaterResult.departedVehicles) {
        addVehicle(vehicle, coreTasks.get(), objectContainer);
    }

    // remove vehicles
    for(auto& sumoIdOfArrived: updaterResult.arrivedVehicles) {
        auto deleter = ObjectRemover::getInstance().getObjectsToDelete("vehicle", mIdMapper(this)[sumoIdOfArrived], objectContainer);
        for(int obj : deleter) {
            coreTasks->objectsToDelete.push_back(obj);
        }
    }

    // update vehicles
    std::list<int> updateList;
    for(auto& sumoIdToUpdate: updaterResult.updateVehicles) {
        int id = mIdMapper(this)[sumoIdToUpdate];
        if (id != 0) {
            updaterResult.updateData->setUpdateForVehicle(sumoIdToUpdate).mObjectId = id;
            updateList.push_back(mIdMapper(this)[sumoIdToUpdate]);
        }
    }

    ActionP updateAction(new Action(std::chrono::milliseconds(1), Action::Kind::START, action->getStartTime()+action->getDuration()+std::chrono::milliseconds(1), updateList));
    updateAction->setActionData(updaterResult.updateData);
    updateAction->setType("SUMO");
    coreTasks->actionsToSchedule.push_back(updateAction);

    return std::move(coreTasks);
}

void SumoMobilityManager::addVehicle(const std::string& vehicle, MobilityManagerTasks* tasks, ConstObjectContainer_ptr objectContainer)
{
    mIdMapper(this)[vehicle] = 0;
    AnyMap a;
    a.add<std::string>("id", vehicle);
    auto vehicleObject = ObjectFactory::getInstance().createObject("vehicle", objectContainer, &a);
    tasks->vehiclesToAdd.push_back(vehicleObject);
}

}
