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

std::shared_ptr<MobilityManagerData> SumoMobilityManager::doVehicleUpdate(std::shared_ptr<Action> action, ObjectContainer_ptr objectList)
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

std::shared_ptr<MobilityManagerData> SumoMobilityManager::executeUpdate(const SumoUpdater::Results& updaterResult, ObjectContainer_ptr objectContainer, std::shared_ptr<Action> action)
{
    // add vehicles
    std::shared_ptr<MobilityManagerData> data = std::make_shared<MobilityManagerData>();
    for(auto& vehicle : updaterResult.departedVehicles) {
        addVehicle(vehicle, data.get(), objectContainer);
    }

    // remove vehicles
    for(auto& vehicle: updaterResult.arrivedVehicles) {
        auto deleter = ObjectRemover::getInstance().getObjectsToDelete("vehicle", mIdMapper[vehicle], objectContainer);
        for(int obj : deleter) {
            data->objectsToDelete.push_back(obj);
        }
    }

    // update vehicles
    std::list<int> updateList;
    for(auto& update: updaterResult.updateVehicles) {
        int id = mIdMapper[update];
        if (id != 0) {
            updaterResult.updateData->getUpdateForVehicle(update).mObjectId = id;
            updateList.push_back(mIdMapper[update]);
        }
    }

    ActionP updateAction(new Action(std::chrono::milliseconds(10), Action::Kind::START, action->getStartTime()+action->getDuration(), updateList));
    updateAction->setActionData(updaterResult.updateData);
    updateAction->setType("SUMO");
    data->actionsToSchedule.push_back(updateAction);

    return std::move(data);
}

void SumoMobilityManager::addVehicle(const std::string& vehicle, MobilityManagerData* data, ObjectContainer_ptr objectContainer)
{
    mIdMapper[vehicle] = 0;
    AnyMap a;
    a.add<std::string>("id", vehicle);
    auto vehicleObject = ObjectFactory::getInstance().createObject("vehicle", objectContainer, &a);
    data->vehiclesToAdd.push_back(vehicleObject);
}

}
