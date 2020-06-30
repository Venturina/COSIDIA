#ifndef _VEHICLE_OBJECT_HPP_SDNFOA
#define _VEHICLE_OBJECT_HPP_SDNFOA

#include "actionData/ActionData.hpp"
#include "objects/BaseObject.hpp"
#include <boost/optional.hpp>


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

    /**
     * This getter is allowed as externalId won't be changed during the simulation run
     */
    virtual std::string getExternalId() { return mExternalId; }
    virtual void setExternalId(std::string id);

    virtual bool isInitialized() override;

private:
    std::string mExternalId = "";
};

} // namespace paresis



#endif /* _VEHICLE_OBJECT_HPP_SDNFOA */