#ifndef _VEHICLE_OBJECT_HPP_SDNFOA
#define _VEHICLE_OBJECT_HPP_SDNFOA

#include "objects/BaseObject.hpp"

namespace paresis
{

class VehicleObject : public BaseObject
{
public:
    VehicleObject(Core* c);
    virtual void startExecution(std::shared_ptr<Action>);
    virtual void endExecution(std::shared_ptr<Action>);
    virtual void initObject(std::shared_ptr<Action>);
    virtual ObjectContext copyContext() {};

private:

};

} // namespace paresis



#endif /* _VEHICLE_OBJECT_HPP_SDNFOA */