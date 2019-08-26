#ifndef _MOBILITY_MANAGER_HPP_SDON
#define _MOBILITY_MANAGER_HPP_SDON

#include "objects/BaseObject.hpp"
#include "objects/ObjectFactory.hpp"
#include <unordered_set>

namespace paresis
{

struct VehicleElement
{
    std::shared_ptr<BaseObject> element;
    std::shared_ptr<Action> action;
};

using Vehicle = std::vector<VehicleElement>;

struct MobilityManagerData
{
    std::list<std::shared_ptr<Action>> actionsToSchedule;
    std::list<Vehicle> vehiclesToAdd;
    std::list<int> objectsToDelete;
};


/**
 * MobilityManager
 * cares about adding, moving and deleting vehicles in the system.
 * usually this can be achieved using SUMO and TraCI
 * This class provides dummy network participants.
 **/
class MobilityManager : public BaseObject
{
public:
    MobilityManager();
    virtual void startExecution(std::shared_ptr<Action>);
    virtual void endExecution(std::shared_ptr<Action>);
    virtual void initObject(std::shared_ptr<Action>);

private:

    std::shared_ptr<MobilityManagerData> doVehicleUpdate(std::shared_ptr<Action> action);
    boost::fibers::future<std::shared_ptr<MobilityManagerData>> mFuture;

    // this has to go to object context possibly
    std::unordered_set<int> mVehicles;

    int updateCount = 0;

    ObjectFactory mFactory;

};



} // namespace paresis
#endif /* _MOBILITY_MANAGER_HPP_SDON */