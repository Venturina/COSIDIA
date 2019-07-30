#ifndef _MOBILITY_MANAGER_HPP_SDON
#define _MOBILITY_MANAGER_HPP_SDON

#include "objects/BaseObject.hpp"
#include <unordered_set>

namespace paresis
{

/**
 * MobilityManager
 * cares about adding, moving and deleting vehicles in the system.
 * usually this can be achieved using SUMO and TraCI
 * This class provides dummy network participants.
 **/
class MobilityManager : public BaseObject
{
public:
    MobilityManager(Core* c) : BaseObject(c) {};
    virtual void startExecution(std::shared_ptr<Action>);
    virtual void endExecution(std::shared_ptr<Action>);
    virtual void initObject(std::shared_ptr<Action>);
    virtual ObjectContext copyContext(){};

private:
    std::unordered_set<int> mVehicles;
};



} // namespace paresis
#endif /* _MOBILITY_MANAGER_HPP_SDON */