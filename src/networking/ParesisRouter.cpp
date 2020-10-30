#include "core/Core.hpp"
#include "networking/ParesisAccessInterface.hpp"
#include "networking/ParesisDccPassThrough.hpp"
#include "networking/ParesisRouter.hpp"
#include "networking/VanetzaDefs.hpp"

#include "vanetza/dcc/bursty_transmit_rate_control.hpp"
#include "vanetza/dcc/flow_control.hpp"
#include "vanetza/dcc/fully_meshed_state_machine.hpp"


#include "loguru/loguru.hpp"


namespace paresis
{

ParesisRouter::ParesisRouter() : mPositionProvider(this), mRuntime(this), mMib(this), mRouter(this), mDccRequestInterface(this), mDccStateMachine(this),
                                mAccessInterface(this), mTransmitRateControl(this), BaseObject()
{
    mObjectName = "ParesisRouter";
    mMib.getElement(this).reset(new vanetza::geonet::MIB);
    mPositionProvider.getElement(this).reset(new ParesisPositionProvider());
}

void ParesisRouter::initObject(std::shared_ptr<Action> action)
{
    mRandomNumber.setElement(getCoreP()->getRandomNumber());

    const auto objList = getCoreP()->getCurrentObjectList();
    auto mobility = getSiblingByName(this, "VehicleObject", objList);
    mVehicleObject = std::static_pointer_cast<VehicleObject>(mobility.lock());

    auto nextAction = createSelfAction(std::chrono::milliseconds(2), action->getStartTime() + std::chrono::milliseconds(mRandomNumber.get()));
    nextAction->setType("initRouter");
    getCoreP()->scheduleAction(nextAction);
}


void ParesisRouter::startExecution(std::shared_ptr<Action> action) {
    if(action->getType() == "update") {
        boost::fibers::packaged_task<RouterUpdateData(std::shared_ptr<Action>, std::shared_ptr<const VehicleObjectContext>, ConstObjectContainer_ptr)>
            pt (std::bind(&ParesisRouter::executeUpdate, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

        mFuture = pt.get_future();
        boost::fibers::fiber(std::move(pt), action, mVehicleObject.lock()->getContext(), getCoreP()->getCurrentObjectList()).detach();
    } else if (action->getType() == "initRouter") {
        boost::fibers::packaged_task<RouterUpdateData(std::shared_ptr<Action>)>
            pt (std::bind(&ParesisRouter::initRouter, this, std::placeholders::_1));

        mFuture = pt.get_future();
        boost::fibers::fiber(std::move(pt), action).detach();
    } else if(action->getType() == "transmission") {
        boost::fibers::packaged_task<RouterUpdateData(std::shared_ptr<Action>, std::shared_ptr<const VehicleObjectContext>, ConstObjectContainer_ptr)>
            pt (std::bind(&ParesisRouter::transmissionReceived, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

        mFuture = pt.get_future();
        boost::fibers::fiber(std::move(pt), action, mVehicleObject.lock()->getContext(), getCoreP()->getCurrentObjectList()).detach();
    } else {
        std::cout << action->getType() << std::endl;
        throw std::runtime_error("ParesisRouter: wrong BeginAction received");
    }
}

RouterUpdateData ParesisRouter::initRouter(std::shared_ptr<Action> action) 
{
    mRuntime.getElement(this) = ParesisRuntime::makeRuntime(action->getStartTime());
    mRouter.getElement(this).reset(new vanetza::geonet::Router(mRuntime(this), mMib(this)));

    mAccessInterface.getElement(this).reset(new ParesisAccessInterface());
    mAccessInterface(this).initializeCache(mObjectName, mObjectId); // TODO: Send to Radio, not to other routers

    // TODO: Add DCC again
    // mDccStateMachine.getElement(this).reset(new vanetza::dcc::FullyMeshedStateMachine());
    // mTransmitRateControl.getElement(this).reset(new vanetza::dcc::BurstyTransmitRateControl(mDccStateMachine(this), mRuntime(this)));
    // mDccRequestInterface.getElement(this).reset(new vanetza::dcc::FlowControl(mRuntime(this), mTransmitRateControl(this), mAccessInterface(this)));

    mDccRequestInterface.getElement(this).reset(new DccPassThrough(mAccessInterface(this)));
    mRouter(this).set_access_interface(mDccRequestInterface.getElement(this).get());

    RouterUpdateData data;

    while(mRuntime(this).getDurationNowToNext().count() <= 0) {
        mRuntime(this).trigger(mRuntime(this).next());
        mAccessInterface(this).dropTransmission();
    }

    scheduleNextUpdate(data, action.get());
    return data;
}

RouterUpdateData ParesisRouter::executeUpdate(std::shared_ptr<Action> action, std::shared_ptr<const VehicleObjectContext> context, ConstObjectContainer_ptr currentObjects)
{
    commonActions(action, context);


    RouterUpdateData data;
    scheduleTransmission(data, action.get(), currentObjects);
    scheduleNextUpdate(data, action.get());
    return data;
}

RouterUpdateData ParesisRouter::transmissionReceived(std::shared_ptr<Action> action, std::shared_ptr<const VehicleObjectContext> context, ConstObjectContainer_ptr currentObjects)
{
    commonActions(action, context);
    DLOG_F(ERROR, "ParesisRouter: received transmission");
    auto transmission = std::dynamic_pointer_cast<const AccesssInterfaceActionData>(action->getActionData());
    //TODO: indicate

    RouterUpdateData data;
    scheduleTransmission(data, action.get(), currentObjects);
    scheduleNextUpdate(data, action.get());
    return data;
}

void ParesisRouter::commonActions(std::shared_ptr<Action> action, std::shared_ptr<const VehicleObjectContext> context)
{
    DLOG_F(ERROR, "current simulation time: %d ", std::chrono::duration_cast<std::chrono::milliseconds>(action->getStartTime()).count());
    mPositionProvider(this).updatePosition(*context);
    mRouter(this).update_position(mPositionProvider(this).position_fix());
    mRuntime(this).triggerAbsolute((action->getStartTime()));
}

void ParesisRouter::endExecution(std::shared_ptr<Action> action) {
    if(action->getType() == "update" || action->getType() == "initRouter" || action->getType() == "transmission") {
        auto data = mFuture.get();
        for(auto action : data.actionsToSchedule) {
            getCoreP()->scheduleAction(std::move(action));
        }
        if(data.actionToDelete) {
            getCoreP()->removeAction(data.actionToDelete);
        }
    } else {
        throw std::runtime_error("ParesisRouter: wrong EndAction received");
    }
}

void ParesisRouter::scheduleNextUpdate(RouterUpdateData& data, const Action* currentAction)
{   //TODO: not fully tested yet
    auto nextTp = mRuntime(this).getDurationStartToNext();
    enforce(!mNextAction || mNextAction->getType() == "update", "ParesisRouter: next action is no update"); // this should never happen

    if(!mNextAction || mNextAction.get() == currentAction) { // new update must be scheduled
        DLOG_F(ERROR, "next Update at: %d milliseconds", std::chrono::duration_cast<std::chrono::milliseconds>(nextTp).count());
        auto nextAction = createSelfAction(std::chrono::milliseconds(2), nextTp);
        nextAction->setType("update");
        data.actionsToSchedule.push_back(nextAction);
        mNextAction = nextAction;
    } else { // nextAction != current -> further decisions required
        if(nextTp == mNextAction->getStartTime()) {  // a update is allready scheduled at the correct time point
            // do nothing here
        } else { // we have to cancel the update
            enforce(mNextAction->getAffected().size()==1, "ParesisRouter: want to cancel action with several recipients");
            #ifdef PARESIS_SAFE
            std::cout << "ObjectId: " << mObjectId << " Type: " << mObjectName << " wants to cancel Action: " << mNextAction->getActionId() << std::endl;
            std::cout << "now: " << std::chrono::duration_cast<std::chrono::milliseconds>(currentAction->getStartTime()).count() << " mNextAction Duration: " << std::chrono::duration_cast<std::chrono::milliseconds>(mNextAction->getStartTime()).count() << " router next action: " << std::chrono::duration_cast<std::chrono::milliseconds>(nextTp).count() << std::endl;
            #endif
            data.actionToDelete = mNextAction;
        }
    }
}

void ParesisRouter::scheduleTransmission(RouterUpdateData& data, const Action* currentAction, ConstObjectContainer_ptr currentObjects)
{
    if(mAccessInterface(this).hasTransmissionRequest()) {
        auto transmission = mAccessInterface(this).getTransmission(currentObjects);
        auto transmissionAction = std::make_shared<Action>(std::chrono::milliseconds(2), Action::Kind::START, currentAction->getEndTime() + std::chrono::milliseconds(1), transmission.first, mObjectId);
        transmissionAction->setType("transmission");
        transmissionAction->setActionData(transmission.second);
        data.actionsToSchedule.push_back(transmissionAction);
    }
}

} // ns paresis