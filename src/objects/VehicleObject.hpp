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
    virtual void setExternalId(std::string id) { externalId = id; }
    virtual std::string getExternalId() { return externalId; }

private:
    std::string externalId = "";
};

} // namespace paresis



#endif /* _VEHICLE_OBJECT_HPP_SDNFOA */