#include "core/Core.hpp"
#include "networking/AccessInterface.hpp"
#include "networking/DccPassThrough.hpp"
#include "networking/Router.hpp"
#include "networking/VanetzaDefs.hpp"

#include "vanetza/dcc/bursty_transmit_rate_control.hpp"
#include "vanetza/dcc/flow_control.hpp"
#include "vanetza/dcc/fully_meshed_state_machine.hpp"


#include "loguru/loguru.hpp"


namespace cosidia
{

Router::Router() : mPositionProvider(this), mRuntime(this), mMib(this), mRouter(this), mDccRequestInterface(this), mDccStateMachine(this),
                                mAccessInterface(this), mTransmitRateControl(this), BaseObject()
{
    mObjectName = "Router";
    mMib.constructElement(this);
    mPositionProvider.constructElement(this);
}

void Router::initObject(std::shared_ptr<Action> action)
{
    mRandomNumber.setElement(getCoreP()->getRandomNumber());

    const auto objList = getCoreP()->getCurrentObjectList();
    auto mobility = getSiblingByName(this, "VehicleObject", objList);
    mVehicleObject = std::static_pointer_cast<VehicleObject>(mobility.lock());

    auto nextAction = createSelfAction(std::chrono::milliseconds(2), action->getStartTime() + std::chrono::milliseconds(mRandomNumber.get()));
    nextAction->setType("initRouter"_sym);
    getCoreP()->scheduleAction(nextAction);
}


void Router::startExecution(std::shared_ptr<Action> action) {
    if(action->getType() == "update"_sym) {
        boost::fibers::packaged_task<RouterUpdateData(std::shared_ptr<Action>, std::shared_ptr<const VehicleObjectContext>, ConstObjectContainer_ptr)>
            pt (std::bind(&Router::executeUpdate, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

        mFuture = pt.get_future();
        boost::fibers::fiber(std::move(pt), action, mVehicleObject.lock()->getContext(), getCoreP()->getCurrentObjectList()).detach();
    } else if (action->getType() == "initRouter"_sym) {
        boost::fibers::packaged_task<RouterUpdateData(std::shared_ptr<Action>)>
            pt (std::bind(&Router::initRouter, this, std::placeholders::_1));

        mFuture = pt.get_future();
        boost::fibers::fiber(std::move(pt), action).detach();
    } else if(action->getType() == "transmission"_sym) {
        boost::fibers::packaged_task<RouterUpdateData(std::shared_ptr<Action>, std::shared_ptr<const VehicleObjectContext>, ConstObjectContainer_ptr)>
            pt (std::bind(&Router::transmissionReceived, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

        mFuture = pt.get_future();
        boost::fibers::fiber(std::move(pt), action, mVehicleObject.lock()->getContext(), getCoreP()->getCurrentObjectList()).detach();
    } else {
        std::cout << action->getType().value() << std::endl;
        throw std::runtime_error("Router: wrong BeginAction received");
    }
}

RouterUpdateData Router::initRouter(std::shared_ptr<Action> action)
{
    mRuntime.constructElement(this, Runtime::makeRuntime(action->getStartTime()));
    mRouter.constructElement(this, mRuntime(this), mMib(this));

    mAccessInterface.constructElement(this);
    mAccessInterface(this).initializeCache(mObjectName, mObjectId); // TODO: Send to Radio, not to other routers

    // TODO: Add DCC again
    // mDccStateMachine.getElement(this).reset(new vanetza::dcc::FullyMeshedStateMachine());
    // mTransmitRateControl.getElement(this).reset(new vanetza::dcc::BurstyTransmitRateControl(mDccStateMachine(this), mRuntime(this)));
    // mDccRequestInterface.getElement(this).reset(new vanetza::dcc::FlowControl(mRuntime(this), mTransmitRateControl(this), mAccessInterface(this)));

    mDccRequestInterface.constructElement<DccPassThrough>(this, mAccessInterface(this));
    mRouter(this).set_access_interface(mDccRequestInterface.getElement(this).get());

    RouterUpdateData data;

    while(mRuntime(this).getDurationNowToNext().count() <= 0) {
        mRuntime(this).trigger(mRuntime(this).next());
        mAccessInterface(this).dropTransmission();
    }

    mInitDone = true;
    scheduleNextUpdate(data, action.get());
    return data;
}

RouterUpdateData Router::executeUpdate(std::shared_ptr<Action> action, std::shared_ptr<const VehicleObjectContext> context, ConstObjectContainer_ptr currentObjects)
{
    RouterUpdateData data;
    commonActions(data, action, context, currentObjects);

    return data;
}

RouterUpdateData Router::transmissionReceived(std::shared_ptr<Action> action, std::shared_ptr<const VehicleObjectContext> context, ConstObjectContainer_ptr currentObjects)
{
    RouterUpdateData data;
    if(mInitDone) {
        commonActions(data, action, context, currentObjects);
        DLOG_F(ERROR, "Router: received transmission");
        auto transmission = std::dynamic_pointer_cast<const AccesssInterfaceActionData>(action->getActionData());
        auto dataRequest = transmission->getDataRequest();
        auto packetPtr = transmission->getPacket();
        vanetza::geonet::Router::UpPacketPtr upPacket { new vanetza::UpPacket(*packetPtr) };
        mRouter(this).indicate(std::move(upPacket), dataRequest.source_addr, dataRequest.destination_addr);
    }
    return data;
}

void Router::commonActions(RouterUpdateData& data, std::shared_ptr<Action> action, std::shared_ptr<const VehicleObjectContext> context, ConstObjectContainer_ptr currentObjects)
{
    DLOG_F(ERROR, "current simulation time: %d ", std::chrono::duration_cast<std::chrono::milliseconds>(action->getStartTime()).count());
    mPositionProvider(this).updatePosition(*context);
    mRouter(this).update_position(mPositionProvider(this).position_fix());
    mRuntime(this).triggerAbsolute((action->getStartTime()));

    scheduleTransmission(data, action.get(), currentObjects);
    scheduleNextUpdate(data, action.get());
}

void Router::endExecution(std::shared_ptr<Action> action) {
    if(action->getType() == "update"_sym || action->getType() == "initRouter"_sym || action->getType() == "transmission"_sym) {
        auto data = mFuture.get();
        for(auto action : data.actionsToSchedule) {
            getCoreP()->scheduleAction(std::move(action));
        }
        if(data.actionToDelete) {
            getCoreP()->removeAction(data.actionToDelete);
        }
    } else {
        throw std::runtime_error("Router: wrong EndAction received");
    }
}

void Router::scheduleNextUpdate(RouterUpdateData& data, const Action* currentAction)
{   //TODO: not fully tested yet
    auto nextTp = mRuntime(this).getDurationStartToNext();
    enforce(!mNextAction || mNextAction->getType() == "update"_sym, "Router: next action is no update"); // this should never happen

    if(!mNextAction || mNextAction.get() == currentAction) { // new update must be scheduled
        DLOG_F(ERROR, "next Update at: %d milliseconds", std::chrono::duration_cast<std::chrono::milliseconds>(nextTp).count());
        auto nextAction = createSelfAction(std::chrono::milliseconds(2), nextTp);
        nextAction->setType("update"_sym);
        data.actionsToSchedule.push_back(nextAction);
        mNextAction = nextAction;
    } else { // nextAction != current -> further decisions required
        if(nextTp == mNextAction->getStartTime()) {  // a update is allready scheduled at the correct time point
            // do nothing here
        } else { // we have to cancel the update
            enforce(mNextAction->getAffected().size()==1, "Router: want to cancel action with several recipients");
            data.actionToDelete = mNextAction;
        }
    }
}

void Router::scheduleTransmission(RouterUpdateData& data, const Action* currentAction, ConstObjectContainer_ptr currentObjects)
{
    if(mAccessInterface(this).hasTransmissionRequest()) {
        auto transmission = mAccessInterface(this).getTransmission(currentObjects);
        auto transmissionAction = std::make_shared<Action>(std::chrono::milliseconds(2), Action::Kind::START, currentAction->getEndTime() + std::chrono::milliseconds(1), transmission.first, mObjectId);
        transmissionAction->setType("transmission"_sym);
        transmissionAction->setActionData(transmission.second);
        data.actionsToSchedule.push_back(transmissionAction);
    }
}

} // ns cosidia