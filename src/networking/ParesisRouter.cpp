#include "core/Core.hpp"
#include "networking/ParesisRouter.hpp"
#include "networking/VanetzaDefs.hpp"

#include "loguru/loguru.hpp"


namespace paresis
{

ParesisRouter::ParesisRouter() : mPositionProvider(this), mRuntime(this), mMib(this), mRouter(this), BaseObject()
{
    mObjectName = "ParesisRouter";
    mMib.getElement(this).reset(new vanetza::geonet::MIB);
    mPositionProvider.getElement(this).reset(new ParesisPositionProvider());
    vanetza::Clock::time_point tv {std::chrono::duration_cast<vanetza::Clock::duration>(getUtcStartTime().time_since_epoch() - utcItsDiff)};
    mItsStartTime.setElement(tv);
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
    mRuntime(this).triggerAbsolute(std::chrono::duration_cast<vanetza::Clock::duration>(action->getStartTime()));
    mRuntime(this).getDurationNowToNext();


    RouterUpdateData data;
    scheduleNextUpdate(data, action.get());
    return data;
}

void ParesisRouter::endExecution(std::shared_ptr<Action> action) {
    if(action->getType() == "update" || action->getType() == "initRouter") {
        auto data = mFuture.get();
        getCoreP()->scheduleAction(data.actionsToSchedule);
    } else {
        throw std::runtime_error("ParesisRouter: wrong action received");
    }
}

void ParesisRouter::scheduleNextUpdate(RouterUpdateData& data, const Action* action)
{
    auto nextTp = mRuntime(this).getDurationStartToNext();
    DLOG_F(ERROR, "next Update at: %d milliseconds", std::chrono::duration_cast<std::chrono::milliseconds>(nextTp).count());
    auto nextAction = createSelfAction(std::chrono::milliseconds(10), nextTp);
    nextAction->setType("update");
    data.actionsToSchedule = nextAction;
}

} // ns paresis