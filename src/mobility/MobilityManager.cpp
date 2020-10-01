#include "core/Core.hpp"
#include "mobility/MobilityManager.hpp"
#include "objects/ObjectRemover.hpp"
#include "objects/VehicleObject.hpp"
#include "utils/AnyMap.hpp"
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
        (std::shared_ptr<Action>, ConstObjectContainer_ptr)> pt
        (std::bind(&MobilityManager::doVehicleUpdate, this, std::placeholders::_1, std::placeholders::_2));

    mFuture = pt.get_future();
    boost::fibers::fiber(std::move(pt), action, objectList).detach();
}

//TODO: iterate object list from newest to oldest object (reverse iteration)
void MobilityManager::fetchVehicleIds(ConstObjectContainer_ptr objectList)
{
    auto data = objectList->getAll();
    for(auto& vehicle : mIdMapper) {
        if(vehicle.second == 0) {
            DLOG_F(ERROR, "found unresolved vehicle");
            for(auto& obj : data) {
                if(obj.second->getObjectName() == "VehicleObject") {
                    auto st = dynamic_cast<VehicleObject*>(obj.second.get())->getExternalId();
                    if(st == vehicle.first) {
                        mIdMapper[vehicle.first] = obj.second->getObjectId();
                        std::string s = "fetched vehicle id: ";
                        s.append(std::to_string(obj.second->getObjectId()));
                        s.append(" for ");
                        s.append(vehicle.first);
                        DLOG_F(ERROR, s.c_str());
                    }
                }
            }
        }
    }
}

std::shared_ptr<MobilityManagerData> MobilityManager::doVehicleUpdate(std::shared_ptr<Action> action, ConstObjectContainer_ptr objectList)
{
    auto data = std::make_shared<MobilityManagerData>();
    DLOG_F(INFO, "MobilityManager: in fiber");
    fetchVehicleIds(objectList);

    if(updateCount % 10 == 0 && updateCount < 100) {
        AnyMap map;
        std::string id = "abc.";
        id.append(std::to_string(updateCount));
        map.add<std::string>("id", "abc.0");
        auto vehicle = ObjectFactory::getInstance().createObject("vehicle", objectList, &map);
        data->vehiclesToAdd.push_back(vehicle);
        mIdMapper[id] = 0;
    }

    if(action->getStartTime() < std::chrono::seconds(20)) {
        auto newAction = std::make_shared<Action>(std::chrono::milliseconds(50),
            Action::Kind::START, action->getStartTime() + std::chrono::milliseconds(100),
            mObjectId); // ugly as hell?

        data->actionsToSchedule.push_back(newAction);
    }

    std::list<std::string> deletedKeys;
    if(updateCount % 5 == 0 && updateCount < 105 && updateCount > 10) {
        for(auto& v : mIdMapper) {
            if(v.second != 0) {
                auto deleter = ObjectRemover::getInstance().getObjectsToDelete("vehicle", v.second, objectList);
                data->objectsToDelete = deleter;
                deletedKeys.push_back(v.first);
            }
        }
    }

    for(auto& k : deletedKeys) {
        mIdMapper.erase(k);
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
    for (auto& vehicle : data->objectsToDelete) {
        DLOG_F(ERROR, "Remove Vehicle");
        getCoreP()->removeObjectFromSimulation(vehicle);
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
