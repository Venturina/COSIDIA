#ifndef _MOBILITY_MANAGER_HPP_SDON
#define _MOBILITY_MANAGER_HPP_SDON

#include "objects/BaseObject.hpp"
#include "objects/ObjectFactory.hpp"
#include "objects/ObjectRemover.hpp"
#include <unordered_set>

namespace paresis
{

struct VehicleElement
{
    std::shared_ptr<BaseObject> element;
    std::shared_ptr<Action> action;
};

//using Vehicle = std::vector<VehicleElement>;




struct MobilityManagerData
{
    std::list<std::shared_ptr<Action>> actionsToSchedule;
    std::list<TemporaryObjectList> vehiclesToAdd;
    std::vector<int> objectsToDelete;
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
    virtual void startExecution(std::shared_ptr<Action>) override;
    virtual void endExecution(std::shared_ptr<Action>) override;
    virtual void initObject(std::shared_ptr<Action>) override;

protected:
    std::map<std::string, int> mIdMapper;
    virtual void fetchVehicleIds(ConstObjectContainer_ptr);

private:

    virtual std::shared_ptr<MobilityManagerData> doVehicleUpdate(std::shared_ptr<Action> action, ConstObjectContainer_ptr);
    boost::fibers::future<std::shared_ptr<MobilityManagerData>> mFuture;

    // this has to go to object context possibly
    std::unordered_set<int> mVehicles;

    int updateCount = 0;

};



} // namespace paresis
#endif /* _MOBILITY_MANAGER_HPP_SDON */