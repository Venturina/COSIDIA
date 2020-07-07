#ifndef _VEHICLE_OBJECT_HPP_SDNFOA
#define _VEHICLE_OBJECT_HPP_SDNFOA

#include "actionData/ActionData.hpp"
#include "objects/BaseObject.hpp"
#include "objects/ObjectContext.hpp"
#include <boost/optional.hpp>



namespace paresis
{

struct VehicleObjectContext
{
    double speed = 0;

    double longitude = 0;
    double latitude = 0;

    VehicleObjectContext& operator=(VehicleObjectContext* other);
};

struct VehicleObjectData
{
    std::shared_ptr<VehicleObjectContext> updatedContext;
};

class VehicleObject : public BaseObject
{
public:
    VehicleObject();
    virtual void startExecution(std::shared_ptr<Action>);
    virtual void endExecution(std::shared_ptr<Action>);
    virtual void initObject(std::shared_ptr<Action>);

    /**
     * This getter is allowed as externalId won't be changed during the simulation run
     */
    virtual std::string getExternalId() { return mExternalId; }
    virtual void setExternalId(std::string id);

    virtual bool isInitialized() override;

private:
    std::string mExternalId = "";
    boost::fibers::future<VehicleObjectData> mFuture;

    VehicleObjectData executeSumoUpdate(std::shared_ptr<Action>, std::shared_ptr<VehicleObjectContext>);

    std::shared_ptr<VehicleObjectContext> mContext;
};

} // namespace paresis



#endif /* _VEHICLE_OBJECT_HPP_SDNFOA */