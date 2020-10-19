#ifndef _MOBILITY_MANAGER_HPP_SDON
#define _MOBILITY_MANAGER_HPP_SDON

#include "objects/BaseObject.hpp"
#include "objects/ObjectFactory.hpp"
#include "objects/ObjectRemover.hpp"
#include "utils/PureLocal.hpp"
#include <unordered_set>

namespace paresis
{

/**
 * Returned by fiber entry method.
 * Contains tasks for core
 */
struct MobilityManagerTasks
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
    PureLocal<std::map<std::string, int>> mIdMapper;

    /**
     * Checks if vehicles in IdMapper are still unresolved
     * If unresolved vehicle was found, ObjectList is searched for Vehicle with unresolved SUMO id
     * When vehicle is found, IdMapper is filled with the corresponding paresis id
     */
    virtual void fetchVehicleIds(ConstObjectContainer_ptr);

private:

    virtual std::shared_ptr<MobilityManagerTasks> doVehicleUpdate(std::shared_ptr<Action> action, ConstObjectContainer_ptr);
    boost::fibers::future<std::shared_ptr<MobilityManagerTasks>> mFuture;

    // this has to go to object context possibly


    int updateCount = 0;

};



} // namespace paresis
#endif /* _MOBILITY_MANAGER_HPP_SDON */