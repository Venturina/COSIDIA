#include "actionData/VehicleUpdateActionData.hpp"
#include "core/Core.hpp"
#include "guiConnector/gui.hpp"
#include "objects/VehicleObject.hpp"
#include "utils/enforce.hpp"

#include <boost/fiber/future.hpp>

#include <loguru/loguru.hpp>
#include <memory>

namespace paresis
{


VehicleObjectContext& VehicleObjectContext::operator=(VehicleObjectContext* other)
{
    speed = other->speed;
    return *this;
}

VehicleObject::VehicleObject() : BaseObject()
{
    mObjectName = "VehicleObject";
}

void VehicleObject::startExecution(std::shared_ptr<Action> action)
{
    //DLOG_F(WARNING, "VehicleObject %s with id %d, executed at %d", mExternalId.c_str(), mObjectId ,std::chrono::duration_cast<std::chrono::milliseconds>(action->getStartTime()).count());
    if(action->getType() == "SUMO") {
        boost::fibers::packaged_task<VehicleObjectData
            (std::shared_ptr<Action>, std::shared_ptr<const VehicleObjectContext>)> pt
            (std::bind(&VehicleObject::executeSumoUpdate, this, std::placeholders::_1, std::placeholders::_2));

            mFuture = pt.get_future();
            boost::fibers::fiber(std::move(pt), action, mContext.getElement()).detach();

    } else {
        auto newAction = createSelfAction(std::chrono::milliseconds(10), action->getStartTime() + std::chrono::milliseconds(100));
        getCoreP()->scheduleAction(newAction);
    }

}

VehicleObjectData VehicleObject::executeSumoUpdate(std::shared_ptr<Action> action, std::shared_ptr<const VehicleObjectContext> context)
{
    auto newContext = std::make_shared<VehicleObjectContext>();
    auto actionData = std::dynamic_pointer_cast<VehicleUpdateActionData>(action->getActionData());
    enforce(actionData, "VehicleObject: Could not cast ActionData to VehicleUpdateActionData");
    enforce(actionData->getUpdateForVehicle(mExternalId.get()).mObjectId == mObjectId, "VehicleObject: ExternalId and ObjectId are not corresponding for VehicleUpdate");

    auto currentUpdate = actionData->getUpdateForVehicle(mExternalId.get());

    newContext->speed = currentUpdate.mSpeed;
    newContext->longitude = currentUpdate.mLongitude;
    newContext->latitude = currentUpdate.mLatitude;

    DLOG_F(WARNING, "vehicle object %s id %d with speed %f and position: %f / %f", mExternalId.get().c_str(), mObjectId, context->speed, context->longitude, context->latitude);
    VehicleObjectData data;
    data.updatedContext = newContext;
    return data;
}

void VehicleObject::endExecution(std::shared_ptr<Action> action)
{
    if(action->getType() == "SUMO") {
        auto data = mFuture.get();
        mContext.swap(data.updatedContext);
    }
    guiUpdateObject(mObjectId, *mContext.getElement());

}


void VehicleObject::initObject(std::shared_ptr<Action> action)
{
    auto number = getCoreP()->getRandomNumber();
    DLOG_F(WARNING, "VehicleObject Init with rnd: %d", number);
    // random number to avoid vehicle synchronisation
    auto newAction = createSelfAction(std::chrono::milliseconds(50), action->getStartTime() + std::chrono::milliseconds(number % 1000));
    mContext.swap(std::make_shared<VehicleObjectContext>());
    getCoreP()->scheduleAction(std::move(newAction));
}

bool VehicleObject::isInitialized()
{
    return BaseObject::isInitialized() && mExternalId.get() != "";
}

void VehicleObject::setExternalId(std::string id)
{
    enforce(!isInitialized(), "VehicleObject: it is not allowed to set ExternalId when initialized");
    mExternalId.setElement(id);
}


} // namespace paresis