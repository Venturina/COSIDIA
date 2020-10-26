#pragma once

#include "networking/ParesisPositionProvider.hpp"
#include "networking/ParesisRuntime.hpp"
#include "objects/BaseObject.hpp"
#include "objects/VehicleObject.hpp"
#include "utils/PureLocal.hpp"

#include "vanetza/geonet/router.hpp"
#include "vanetza/geonet/mib.hpp"

namespace paresis
{

struct RouterUpdateData
{
    std::shared_ptr<Action> actionsToSchedule;
    std::shared_ptr<Action> actionToDelete;
};

class ParesisRouter : public BaseObject
{
public:
    ParesisRouter();
    virtual void startExecution(std::shared_ptr<Action>);
    virtual void endExecution(std::shared_ptr<Action>);
    virtual void initObject(std::shared_ptr<Action>);

private:

    void scheduleNextUpdate(RouterUpdateData&, const Action*);
    RouterUpdateData executeUpdate(std::shared_ptr<Action> action, std::shared_ptr<const VehicleObjectContext> context);

    RouterUpdateData initRouter(std::shared_ptr<Action> action);

    /* do not touch this variables from other thread than main */
    std::weak_ptr<VehicleObject> mVehicleObject;
    boost::fibers::future<RouterUpdateData> mFuture;

    /* pure locals, do not return */
    PureLocal<ParesisPositionProvider> mPositionProvider;
    PureLocal<ParesisRuntime> mRuntime;
    PureLocal<vanetza::geonet::MIB> mMib;
    PureLocal<vanetza::geonet::Router> mRouter;

    ReadOnly<vanetza::Clock::time_point> mItsStartTime;
    ReadOnly<int> mRandomNumber;
    //runtime
    //router

};

} // namespace paresis