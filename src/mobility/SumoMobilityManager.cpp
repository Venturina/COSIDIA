#include "core/DurationAction.hpp"
#include "mobility/SumoMobilityManager.hpp"
#include "objects/VehicleObject.hpp"
#include "traci/PosixLauncher.h"
#include "core/Config.hpp"
#include "core/Core.hpp"

#include "loguru/loguru.hpp"

namespace cosidia
{
void SumoMobilityManager::initObject(std::shared_ptr<Action> action)
{
    mUpdateInterval = std::chrono::milliseconds(100);
    auto nextAction = std::make_shared<DurationAction>(std::chrono::milliseconds(10), action->getStartTime()+mUpdateInterval, mObjectId, mObjectId);
    nextAction->scheduleStartHandler();
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
    auto res = mUpdater->step(std::chrono::duration_cast<std::chrono::milliseconds>(t.time_since_epoch()));
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
        auto deleter = ObjectRemover::getInstance().getObjectsToDelete("vehicle", mIdMapper(this).at(sumoIdOfArrived), objectContainer);
        for(ObjectId obj : deleter) {
            coreTasks->objectsToDelete.push_back(obj);
        }
    }

    // update vehicles

    for(auto& sumoIdToUpdate: updaterResult.updateVehicles) {
        ObjectId id = mIdMapper(this).at(sumoIdToUpdate);
        if (id.valid()) {
            updaterResult.updateData->setUpdateForVehicle(sumoIdToUpdate).mObjectId = id;
            ActionP updateAction(new DurationAction(std::chrono::milliseconds(1), action->getStartTime()+action->getDuration()+std::chrono::milliseconds(1), id, mObjectId));
            updateAction->setActionData(updaterResult.updateData);
            updateAction->setType("SUMO"_sym);
            coreTasks->actionsToSchedule.push_back(updateAction);
        }
    }

    return std::move(coreTasks);
}

void SumoMobilityManager::addVehicle(const std::string& vehicle, MobilityManagerTasks* tasks, ConstObjectContainer_ptr objectContainer)
{
    mIdMapper(this).emplace(vehicle, ObjectId::stub());
    AnyMap a;
    a.add<std::string>("id", vehicle);
    auto vehicleObject = ObjectFactory::getInstance().createObject("vehicle", objectContainer, &a);
    tasks->vehiclesToAdd.push_back(vehicleObject);
}

}
