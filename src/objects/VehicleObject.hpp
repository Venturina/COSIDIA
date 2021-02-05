#ifndef _VEHICLE_OBJECT_HPP_SDNFOA
#define _VEHICLE_OBJECT_HPP_SDNFOA

#include "actionData/ActionData.hpp"
#include "objects/BaseObject.hpp"
#include "objects/ObjectContext.hpp"
#include "utils/ContextContainer.hpp"
#include "utils/Geometry.h"
#include "utils/ReadOnly.hpp"
#include <boost/optional.hpp>



namespace cosidia
{

struct VehicleObjectContext
{
    double speed = 0;   /* meter per second */

    GeoPosition geo;    /* degree */
    Position position;  /* meter */

    double heading = 0; /* plane angle (-180 - 180)  */

    SimClock::time_point lastUpdate;
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
    virtual std::string getExternalId() { return mExternalId.get(); }
    virtual void setExternalId(std::string id);

    virtual std::shared_ptr<const VehicleObjectContext> getContext() { return mContext.getElement(); }

    virtual bool isInitialized() override;

private:
    ReadOnly<std::string> mExternalId;
    boost::fibers::future<VehicleObjectData> mFuture;

    VehicleObjectData executeSumoUpdate(std::shared_ptr<Action>, std::shared_ptr<const VehicleObjectContext>);

    ContextContainer<VehicleObjectContext> mContext;
};

} // namespace cosidia



#endif /* _VEHICLE_OBJECT_HPP_SDNFOA */