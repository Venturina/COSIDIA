#include "core/Core.hpp"
#include "networking/ParesisRouter.hpp"

namespace paresis
{

ParesisRouter::ParesisRouter() : mPositionProvider(this), BaseObject()
{
    mObjectName = "ParesisRouter";
}

void ParesisRouter::initObject(std::shared_ptr<Action> action)
{
    const auto objList = getCoreP()->getCurrentObjectList();
    auto mobility = getSiblingByName(this, "VehicleObject", objList);
    mVehicleObject = std::static_pointer_cast<VehicleObject>(mobility.lock());

    auto nextAction = createSelfAction(std::chrono::milliseconds(10), action->getEndTime() + std::chrono::milliseconds(10));
    nextAction->setType("update");
    getCoreP()->scheduleAction(nextAction);
}


void ParesisRouter::startExecution(std::shared_ptr<Action> action) {
    if(action->getType() == "update") {
        boost::fibers::packaged_task<RouterUpdateData(std::shared_ptr<Action>, std::shared_ptr<const VehicleObjectContext>)>
            pt (std::bind(&ParesisRouter::executeUpdate, this, std::placeholders::_1, std::placeholders::_2));

        mFuture = pt.get_future();
        boost::fibers::fiber(std::move(pt), action, mVehicleObject.lock()->getContext()).detach();
    } else {
        std::cout << action->getType() << std::endl;
        throw std::runtime_error("ParesisRouter: wrong action received");
    }
}

RouterUpdateData ParesisRouter::executeUpdate(std::shared_ptr<Action> action, std::shared_ptr<const VehicleObjectContext> context)
{
    

    RouterUpdateData data;
    scheduleNextUpdate(data, action.get());
    return data;
}

void ParesisRouter::endExecution(std::shared_ptr<Action> action) {
    if(action->getType() == "update") {
        auto data = mFuture.get();
        getCoreP()->scheduleAction(data.actionsToSchedule);
    } else {
        throw std::runtime_error("ParesisRouter: wrong action received");
    }
}

void ParesisRouter::scheduleNextUpdate(RouterUpdateData& data, const Action* action)
{
    auto nextAction = createSelfAction(std::chrono::milliseconds(10), action->getEndTime() + std::chrono::milliseconds(100));
    nextAction->setType("update");
    data.actionsToSchedule = nextAction;
}

} // ns paresis