#include "core/Core.hpp"
#include "networking/ParesisAccessInterface.hpp"
#include "networking/ParesisRouter.hpp"
#include "networking/VanetzaDefs.hpp"

#include "vanetza/dcc/bursty_transmit_rate_control.hpp"
#include "vanetza/dcc/flow_control.hpp"
#include "vanetza/dcc/fully_meshed_state_machine.hpp"

#include "loguru/loguru.hpp"


namespace paresis
{

ParesisRouter::ParesisRouter() : mPositionProvider(this), mRuntime(this), mMib(this), mRouter(this), mDccRequestInterface(this), mDccStateMachine(this),
                                mAccessInterface(this), mTransmitRateControl(this), mObjectCache(this), BaseObject()
{
    mObjectName = "ParesisRouter";
    mMib.getElement(this).reset(new vanetza::geonet::MIB);
    mPositionProvider.getElement(this).reset(new ParesisPositionProvider());
    mObjectCache.getElement(this).reset(new SimpleObjectCache());
}

void ParesisRouter::initObject(std::shared_ptr<Action> action)
{
    mRandomNumber.setElement(getCoreP()->getRandomNumber());

    const auto objList = getCoreP()->getCurrentObjectList();
    auto mobility = getSiblingByName(this, "VehicleObject", objList);
    mVehicleObject = std::static_pointer_cast<VehicleObject>(mobility.lock());

    auto nextAction = createSelfAction(std::chrono::milliseconds(10), action->getStartTime() + std::chrono::milliseconds(mRandomNumber.get()));
    nextAction->setType("initRouter");
    getCoreP()->scheduleAction(nextAction);
}


void ParesisRouter::startExecution(std::shared_ptr<Action> action) {
    if(action->getType() == "update") {
        boost::fibers::packaged_task<RouterUpdateData(std::shared_ptr<Action>, std::shared_ptr<const VehicleObjectContext>)>
            pt (std::bind(&ParesisRouter::executeUpdate, this, std::placeholders::_1, std::placeholders::_2));

        mFuture = pt.get_future();
        boost::fibers::fiber(std::move(pt), action, mVehicleObject.lock()->getContext()).detach();
    } else if (action->getType() == "initRouter") {
        boost::fibers::packaged_task<RouterUpdateData(std::shared_ptr<Action>)>
            pt (std::bind(&ParesisRouter::initRouter, this, std::placeholders::_1));

        mFuture = pt.get_future();
        boost::fibers::fiber(std::move(pt), action).detach();
    } else {
        std::cout << action->getType() << std::endl;
        throw std::runtime_error("ParesisRouter: wrong action received");
    }
}

RouterUpdateData ParesisRouter::initRouter(std::shared_ptr<Action> action) {
    mRuntime.getElement(this) = ParesisRuntime::makeRuntime(action->getStartTime());
    mRouter.getElement(this).reset(new vanetza::geonet::Router(mRuntime(this), mMib(this)));

    mAccessInterface.getElement(this).reset(new ParesisAccessInterface());
    mDccStateMachine.getElement(this).reset(new vanetza::dcc::FullyMeshedStateMachine());
    mTransmitRateControl.getElement(this).reset(new vanetza::dcc::BurstyTransmitRateControl(mDccStateMachine(this), mRuntime(this)));
    mDccRequestInterface.getElement(this).reset(new vanetza::dcc::FlowControl(mRuntime(this), mTransmitRateControl(this), mAccessInterface(this)));
    mRouter(this).set_access_interface(mDccRequestInterface.getElement(this).get());


    while(mRuntime(this).getDurationNowToNext().count() <= 0) {
        mRuntime(this).trigger(mRuntime(this).next());
    }

    RouterUpdateData data;
    scheduleNextUpdate(data, action.get());
    return data;
}

RouterUpdateData ParesisRouter::executeUpdate(std::shared_ptr<Action> action, std::shared_ptr<const VehicleObjectContext> context)
{
    DLOG_F(ERROR, "current simulation time: %d ", std::chrono::duration_cast<std::chrono::milliseconds>(action->getStartTime()).count());
    mPositionProvider(this).updatePosition(*context);
    mRouter(this).update_position(mPositionProvider(this).position_fix());
    mRuntime(this).triggerAbsolute((action->getStartTime()));
    mRuntime(this).getDurationNowToNext();


    RouterUpdateData data;
    scheduleNextUpdate(data, action.get());
    return data;
}

void ParesisRouter::endExecution(std::shared_ptr<Action> action) {
    if(action->getType() == "update" || action->getType() == "initRouter") {
        auto data = mFuture.get();
        for(auto action : data.actionsToSchedule) {
            getCoreP()->scheduleAction(std::move(action));
        }
    } else {
        throw std::runtime_error("ParesisRouter: wrong action received");
    }
}

void ParesisRouter::scheduleNextUpdate(RouterUpdateData& data, const Action* currentAction)
{   //TODO: not fully tested yet
    auto nextTp = mRuntime(this).getDurationStartToNext();

    if(mNextAction && mNextAction.get() != currentAction && nextTp != mNextAction->getStartTime()) {
        data.actionToDelete = mNextAction;
    }

    DLOG_F(ERROR, "next Update at: %d milliseconds", std::chrono::duration_cast<std::chrono::milliseconds>(nextTp).count());
    auto nextAction = createSelfAction(std::chrono::milliseconds(10), nextTp);
    nextAction->setType("update");
    data.actionsToSchedule.push_back(nextAction);
    mNextAction = nextAction;
}

} // ns paresis