#pragma once

#include "application/InternalCaService.hpp"
#include "networking/PositionProvider.hpp"
#include "networking/Runtime.hpp"
#include "networking/AccessInterface.hpp"
#include "networking/ActiveActionTracker.hpp"
#include "objects/BaseObject.hpp"
#include "objects/ObjectCache.hpp"
#include "objects/VehicleObject.hpp"
#include "radio/Radio.hpp"
#include "utils/PureLocal.hpp"

#include "vanetza/dcc/interface.hpp"
#include "vanetza/dcc/state_machine.hpp"
#include "vanetza/dcc/transmit_rate_control.hpp"

#include "vanetza/geonet/router.hpp"
#include "vanetza/geonet/mib.hpp"

namespace cosidia
{

struct RouterUpdateData
{
    std::list<std::shared_ptr<Action>> actionsToSchedule;
    std::shared_ptr<Action> actionToDelete;
    std::shared_ptr<Transmission> transmission;
};

class Router : public BaseObject
{
public:
    Router();
    virtual void startExecution(std::shared_ptr<Action>);
    virtual void endExecution(std::shared_ptr<Action>);
    virtual void initObject(std::shared_ptr<Action>);

private:
    RouterUpdateData executeUpdate(std::shared_ptr<Action> action, std::shared_ptr<const VehicleObjectContext> context, ConstObjectContainer_ptr);
    RouterUpdateData transmissionReceived(std::shared_ptr<Action> action, std::shared_ptr<const VehicleObjectContext> context, ConstObjectContainer_ptr);
    RouterUpdateData caUpdate(std::shared_ptr<Action> action, std::shared_ptr<const VehicleObjectContext> context, ConstObjectContainer_ptr);

    RouterUpdateData initRouter(std::shared_ptr<Action> action);

    void scheduleNextUpdate(RouterUpdateData&, const Action*);
    void scheduleTransmission(RouterUpdateData&, const Action*, ConstObjectContainer_ptr);
    void commonActions(RouterUpdateData&, std::shared_ptr<Action>, std::shared_ptr<const VehicleObjectContext>, ConstObjectContainer_ptr);

    /* do not touch this variables from other thread than main */
    std::weak_ptr<VehicleObject> mVehicleObject;
    std::weak_ptr<Radio> mRadioObject;
    boost::fibers::future<RouterUpdateData> mFuture;

    /* pure locals, do not return */
    PureLocal<PositionProvider> mPositionProvider;
    PureLocal<Runtime> mRuntime;
    PureLocal<vanetza::geonet::MIB> mMib;
    PureLocal<vanetza::geonet::Router> mRouter;

    PureLocal<vanetza::dcc::RequestInterface> mDccRequestInterface;
    PureLocal<vanetza::dcc::StateMachine> mDccStateMachine;
    PureLocal<AccessInterface> mAccessInterface;
    PureLocal<vanetza::dcc::TransmitRateControl> mTransmitRateControl;

    PureLocal<InternalCaService> mCaService;

    bool mInitDone = false;
    ActiveActionTracker mTracker;
    ReadOnly<int> mRandomNumber;
    //runtime
    //router

};

} // namespace cosidia