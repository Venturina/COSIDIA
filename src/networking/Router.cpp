#include "application/ServiceActionData.hpp"
#include "core/Core.hpp"
#include "networking/AccessInterface.hpp"
#include "networking/DccPassThrough.hpp"
#include "networking/Router.hpp"
#include "networking/VanetzaDefs.hpp"
#include "output/TimingScope.hpp"

#include <vanetza/btp/header.hpp>
#include <vanetza/btp/header_conversion.hpp>
#include <vanetza/common/byte_buffer_convertible.hpp>
#include <vanetza/dcc/bursty_transmit_rate_control.hpp>
#include <vanetza/dcc/flow_control.hpp>
#include <vanetza/dcc/fully_meshed_state_machine.hpp>
#include <vanetza/geonet/data_confirm.hpp>

#include "boost/thread/futures/future_error.hpp"
#include "loguru/loguru.hpp"


namespace cosidia
{

Router::Router() : mPositionProvider(this), mRuntime(this), mMib(this), mRouter(this), mDccRequestInterface(this), mDccStateMachine(this),
                                mAccessInterface(this), mTransmitRateControl(this), mCaService(this), BaseObject()
{
    mObjectName = "Router";
    mMib.constructElement(this);
    mPositionProvider.constructElement(this);
    mCaService.constructElement(this, this);
}

void Router::initObject(std::shared_ptr<Action> action)
{
    mRandomNumber.setElement(getCoreP()->getRandomNumber());

    const auto objList = getCoreP()->getCurrentObjectList();
    auto mobility = getSiblingByName(this, "VehicleObject", objList);
    mVehicleObject = std::static_pointer_cast<VehicleObject>(mobility.lock());
    mRadioObject = std::static_pointer_cast<Radio>(objList->getUnique("Radio"));

    auto nextAction = createSelfAction(std::chrono::milliseconds(2), action->getStartTime() + std::chrono::milliseconds(mRandomNumber.get()));
    nextAction->setType("initRouter"_sym);
    nextAction->scheduleStartHandler();
}


void Router::startExecution(std::shared_ptr<Action> action) {
    if(action->getType() == "caUpdate"_sym) {
        boost::fibers::packaged_task<RouterUpdateData(std::shared_ptr<Action>, std::shared_ptr<const VehicleObjectContext>, ConstObjectContainer_ptr)>
            pt (std::bind(&Router::caUpdate, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

        mFuture = pt.get_future();
        boost::fibers::fiber(std::move(pt), action, mVehicleObject.lock()->getContext(), getCoreP()->getCurrentObjectList()).detach();
        return;
    }  else if (action->getType() == "initRouter"_sym) {
        boost::fibers::packaged_task<RouterUpdateData(std::shared_ptr<Action>)>
            pt (std::bind(&Router::initRouter, this, std::placeholders::_1));

        mFuture = pt.get_future();
        boost::fibers::fiber(std::move(pt), action).detach();
        return;
    } else if (mTracker.isStart(action.get())) {
        if(action->getType() == "update"_sym) {
            boost::fibers::packaged_task<RouterUpdateData(std::shared_ptr<Action>, std::shared_ptr<const VehicleObjectContext>, ConstObjectContainer_ptr)>
                pt (std::bind(&Router::executeUpdate, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

            mFuture = pt.get_future();
            boost::fibers::fiber(std::move(pt), action, mVehicleObject.lock()->getContext(), getCoreP()->getCurrentObjectList()).detach();
            return;
        } else if(action->getType() == "indication"_sym) {
            boost::fibers::packaged_task<RouterUpdateData(std::shared_ptr<Action>, std::shared_ptr<const VehicleObjectContext>, ConstObjectContainer_ptr)>
                pt (std::bind(&Router::transmissionReceived, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

            mFuture = pt.get_future();
            boost::fibers::fiber(std::move(pt), action, mVehicleObject.lock()->getContext(), getCoreP()->getCurrentObjectList()).detach();
            return;
        }
        std::cout << action->getType().value() << std::endl;
        throw std::runtime_error("Router: wrong BeginAction received");
    } else {
        enforce(!mTracker.isStart(action.get()), "Router: Action thing went wrong")
    }
}

RouterUpdateData Router::initRouter(std::shared_ptr<Action> action)
{
    COSIDIA_TIMING(action);
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

    auto caAction = createSelfAction(std::chrono::milliseconds(2), action->getStartTime()+std::chrono::milliseconds(100));
    caAction->setType("caUpdate"_sym);
    data.actionsToSchedule.push_back(caAction);
    scheduleNextUpdate(data, action.get());
    return data;
}

RouterUpdateData Router::executeUpdate(std::shared_ptr<Action> action, std::shared_ptr<const VehicleObjectContext> context, ConstObjectContainer_ptr currentObjects)
{
    COSIDIA_TIMING(action);
    RouterUpdateData data;
    commonActions(data, action, context, currentObjects);

    return data;
}

RouterUpdateData Router::caUpdate(std::shared_ptr<Action> action, std::shared_ptr<const VehicleObjectContext> context, ConstObjectContainer_ptr currentObjects)
{
    COSIDIA_TIMING(action);
    RouterUpdateData data;
    auto fix = &mPositionProvider.getElement(this)->position_fix();
    std::cout << fix->course.value().value() * 10 << std::endl;
    if(mInitDone && fix->course.value().value() >= 0) {
        DLOG_F(WARNING, "Router %d: CA Update", mObjectId.raw());
        auto transmitssion = mCaService.getElement(this)->triggerCam(fix, action.get(), &mRouter(this), mMib(this));
    }
    auto caAction = createSelfAction(std::chrono::milliseconds(2), action->getStartTime()+std::chrono::milliseconds(100));
    caAction->setType("caUpdate"_sym);
    data.actionsToSchedule.push_back(caAction);
    commonActions(data, action, context, currentObjects);
    return data;
}

RouterUpdateData Router::transmissionReceived(std::shared_ptr<Action> action, std::shared_ptr<const VehicleObjectContext> context, ConstObjectContainer_ptr currentObjects)
{
    COSIDIA_TIMING(action);
    RouterUpdateData data;
    if(mInitDone) {
        DLOG_F(ERROR, "Router %d: received transmission", mObjectId.raw());
        auto transmission = std::dynamic_pointer_cast<const Transmission>(action->getActionData());
        auto dataRequest = transmission->getRequest();
        auto packetPtr = transmission->getPacket();
        vanetza::geonet::Router::UpPacketPtr upPacket { new vanetza::UpPacket(packetPtr) };
        mRouter(this).indicate(std::move(upPacket), dataRequest.source_addr, dataRequest.destination_addr);
        commonActions(data, action, context, currentObjects);
    } else {
        return data;
    } 
    return data;
}

void Router::commonActions(RouterUpdateData& data, std::shared_ptr<Action> action, std::shared_ptr<const VehicleObjectContext> context, ConstObjectContainer_ptr currentObjects)
{
    DLOG_F(ERROR, "current simulation time: %d, Router %d, Action id: %d", SimClock::getMilliseconds(action->getStartTime()), mObjectId.raw(), action->getActionId());
    mPositionProvider(this).updatePosition(*context);
    mRouter(this).update_position(mPositionProvider(this).position_fix());
    mRuntime(this).trigger((action->getStartTime()));

    scheduleTransmission(data, action.get(), currentObjects);
    scheduleNextUpdate(data, action.get());
}

void Router::endExecution(std::shared_ptr<Action> action) {
    if((action->getType() == "update"_sym || action->getType() == "indication"_sym)) {
        if (!mTracker.isEnd(action.get())) {
            return;
        }

        try {
            auto data = mFuture.get();
            for(auto action : data.actionsToSchedule) {
                action->scheduleStartHandler();
            }
            if(data.actionToDelete) {
                getCoreP()->removeAction(data.actionToDelete);
            }
            if(data.transmission) {
                data.transmission->setupReceiverContext();
                data.transmission->addEmitter(mObjectId, mVehicleObject.lock()->getContext());
            }
        } catch (const std::exception& e) {
            action->prettyPrint();
            DLOG_F(ERROR, "Thrown exception '%s' on action %d at object %s with ID %d", e.what(), action->getActionId(), mObjectName.c_str(),  mObjectId);
            throw e;
        }

    } else if (action->getType() == "caUpdate"_sym || action->getType() == "initRouter"_sym) {
        try {
            auto data = mFuture.get();
            for(auto action : data.actionsToSchedule) {
                action->scheduleStartHandler();
            }
            if(data.actionToDelete) {
                getCoreP()->removeAction(data.actionToDelete);
            }
            if(data.transmission) {
                data.transmission->setupReceiverContext();
                data.transmission->addEmitter(mObjectId, mVehicleObject.lock()->getContext());
            }
        } catch (const std::exception& e) {
            mHistory->prettyPrint();
            action->prettyPrint();
            DLOG_F(ERROR, "Thrown exception '%s' on action %d at object %s with ID %d", e.what(), action->getActionId(), mObjectName.c_str(),  mObjectId);
            throw e;
        }
    } else {
        throw std::runtime_error("Router: wrong EndAction received");
    }
}

void Router::scheduleNextUpdate(RouterUpdateData& data, const Action* currentAction)
{   //TODO: not fully tested yet
    auto nextTp = mRuntime(this).getNextStart();
    if(nextTp != mTracker.nextStart()) {
    //enforce(!mTracker.getEnd() || mTracker.getEnd() == currentAction, "Router: messed up with ActionTracker");
    //enforce(!mNextAction || mNextAction->getType() == "update"_sym, "Router: next action is no update"); // this should never happen

    //if(mTracker.isStart(currentAction)) { // new update must be scheduled
        DLOG_F(ERROR, "next Update at: %d milliseconds", SimClock::getMilliseconds(nextTp));
        auto nextAction = createSelfAction(std::chrono::milliseconds(2), nextTp);
        nextAction->setType("update"_sym);
        data.actionsToSchedule.push_back(nextAction);
        mTracker.setNext(nextAction.get());
    }
        //mNextAction = nextAction;
    //} else { // nextAction != current -> further decisions required
        //if(nextTp == mNextAction->getStartTime()) {  // a update is allready scheduled at the correct time point
            // do nothing here
        //} else { // we have to cancel the update
            //data.actionToDelete = mNextAction;
        //}
    //}
}

void Router::scheduleTransmission(RouterUpdateData& data, const Action* currentAction, ConstObjectContainer_ptr currentObjects)
{
    if(mAccessInterface(this).hasTransmissionRequest()) {
        auto transmission = mAccessInterface(this).getTransmission(currentObjects);
        data.transmission = transmission;
        auto transmissionAction = ActionFactory<DurationAction>::create(std::chrono::milliseconds(2), currentAction->getEndTime() + std::chrono::milliseconds(1), mRadioObject.lock()->getObjectId(), mObjectId);
        transmissionAction->setType("transmissionStart"_sym);
        transmissionAction->setActionData(transmission);
        data.actionsToSchedule.push_back(transmissionAction);
    }
}

} // ns cosidia