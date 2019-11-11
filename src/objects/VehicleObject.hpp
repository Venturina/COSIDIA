#ifndef _VEHICLE_OBJECT_HPP_SDNFOA
#define _VEHICLE_OBJECT_HPP_SDNFOA

#include "objects/BaseObject.hpp"

namespace paresis
{

class VehicleObject : public BaseObject
{
public:
    VehicleObject();
    virtual void startExecution(std::shared_ptr<Action>);
    virtual void endExecution(std::shared_ptr<Action>);
    virtual void initObject(std::shared_ptr<Action>);
    virtual ObjectContext copyContext() {};
    virtual void setExternalId(int id) { externalId = id; }

private:
    int externalId = 0;
};

} // namespace paresis



#endif /* _VEHICLE_OBJECT_HPP_SDNFOA */