#include "core/Core.hpp"
#include "core/DurationAction.hpp"
#include "mobility/MobilityManager.hpp"
#include "objects/ObjectRemover.hpp"
#include "objects/VehicleObject.hpp"
#include "output/TimingScope.hpp"
#include "utils/AnyMap.hpp"
#include <boost/fiber/future.hpp>
#include <loguru/loguru.hpp>


namespace cosidia
{

MobilityManager::MobilityManager() : mIdMapper(this), BaseObject()
{
    mObjectName = "MobilityManager";
    mIdMapper.constructElement(this);
}

void MobilityManager::startExecution(std::shared_ptr<Action> action)
{
    DLOG_F(INFO, "MobilityManager update");
    auto objectList = getCoreP()->getCurrentObjectList();

    boost::fibers::packaged_task<std::shared_ptr<MobilityManagerTasks>
        (std::shared_ptr<Action>, ConstObjectContainer_ptr)> pt
        (std::bind(&MobilityManager::doVehicleUpdate, this, std::placeholders::_1, std::placeholders::_2));

    mFuture = pt.get_future();
    boost::fibers::fiber(std::move(pt), action, objectList).detach();
}

//TODO: iterate object list from newest to oldest object (reverse iteration)
void MobilityManager::fetchVehicleIds(ConstObjectContainer_ptr objectList)
{
    auto data = objectList->getAll();
    for (auto it = mIdMapper(this).begin(); it != mIdMapper(this).end(); ++it) {
        ObjectId& id = it->second;
        if(!id.valid()) {
            DLOG_F(ERROR, "found unresolved vehicle");
            for(auto& obj : data) {
                if(obj.second->getObjectName() == "VehicleObject") {
                    auto st = dynamic_cast<VehicleObject*>(obj.second.get())->getExternalId();
                    if(st == it->first) {
                        // replace stub object id by real one
                        id = obj.second->getObjectId();
                        std::string s = "fetched vehicle id: ";
                        s.append(std::to_string(obj.second->getObjectId().raw()));
                        s.append(" for ");
                        s.append(it->first);
                        DLOG_F(ERROR, s.c_str());
                    }
                }
            }
        }
    }
}

std::shared_ptr<MobilityManagerTasks> MobilityManager::doVehicleUpdate(std::shared_ptr<Action> action, ConstObjectContainer_ptr objectList)
{
    COSIDIA_TIMING(action);
    auto data = std::make_shared<MobilityManagerTasks>();
    DLOG_F(INFO, "MobilityManager: in fiber");
    fetchVehicleIds(objectList);

    if(updateCount % 10 == 0 && updateCount < 100) {
        AnyMap map;
        std::string id = "abc.";
        id.append(std::to_string(updateCount));
        map.add<std::string>("id", "abc.0");
        auto vehicle = ObjectFactory::getInstance().createObject("vehicle", objectList, &map);
        data->vehiclesToAdd.push_back(vehicle);
        mIdMapper(this).emplace(id, ObjectId::stub());
    }

    if(action->getStartTime() < SimClock::atSecond(20)) {
        auto newAction = ActionFactory<DurationAction>::create(std::chrono::milliseconds(50), action->getStartTime() + std::chrono::milliseconds(100),
            mObjectId, mObjectId); // ugly as hell?
        newAction->setType("MobilityManager:Update"_sym);

        data->actionsToSchedule.push_back(newAction);
    }

    std::list<std::string> deletedKeys;
    if(updateCount % 5 == 0 && updateCount < 105 && updateCount > 10) {
        for(auto& v : mIdMapper(this)) {
            if(v.second.valid()) {
                auto deleter = ObjectRemover::getInstance().getObjectsToDelete("vehicle", v.second, objectList);
                data->objectsToDelete = deleter;
                deletedKeys.push_back(v.first);
            }
        }
    }

    for(auto& k : deletedKeys) {
        mIdMapper(this).erase(k);
    }

    updateCount++;
    return data;
}

void MobilityManager::endExecution(std::shared_ptr<Action> action)
{
    auto data = mFuture.get();
    for (auto& vehicle : data->vehiclesToAdd) {
        DLOG_F(ERROR, "Add vehicle");
        vehicle.resolveAndStart(action->getEndTime());
    }
    for(auto& a : data->actionsToSchedule) {
        a->scheduleStartHandler();
    }
    for (auto& vehicle : data->objectsToDelete) {
        DLOG_F(ERROR, "Remove Vehicle");
        getCoreP()->removeObjectFromSimulation(vehicle);
    }
}

void MobilityManager::initObject(std::shared_ptr<Action> action)
{
    DLOG_F(INFO, "Id of MobilityManager: %d", mObjectId);

    auto newAction = ActionFactory<DurationAction>::create(std::chrono::milliseconds(50), action->getStartTime() + std::chrono::milliseconds(100), mObjectId, mObjectId); // ugly as hell?
    newAction->scheduleStartHandler();

}

} // namespace cosidia
