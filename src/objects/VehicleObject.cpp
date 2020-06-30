#include "core/Core.hpp"
#include "objects/VehicleObject.hpp"
#include "utils/enforce.hpp"

#include <loguru/loguru.hpp>

namespace paresis
{

VehicleObject::VehicleObject() : BaseObject()
{
    mObjectName = "VehicleObject";
}

void VehicleObject::startExecution(std::shared_ptr<Action> action)
{
    std::cout << mExternalId << std::endl;
    DLOG_F(WARNING, "VehicleObject %s, executed at %d", mExternalId.c_str() ,std::chrono::duration_cast<std::chrono::milliseconds>(action->getStartTime()).count());
    if(action->getType() != "SUMO") {
        auto newAction = createSelfAction(std::chrono::milliseconds(10), action->getStartTime() + std::chrono::milliseconds(100));
        getCoreP()->scheduleAction(newAction);
    }

}

void VehicleObject::endExecution(std::shared_ptr<Action> action)
{

}


void VehicleObject::initObject(std::shared_ptr<Action> action)
{
    auto number = getCoreP()->getRandomNumber();
    DLOG_F(WARNING, "VehicleObject Init with rnd: %d", number);
    // random number to avoid vehicle synchronisation
    auto newAction = createSelfAction(std::chrono::milliseconds(50), action->getStartTime() + std::chrono::milliseconds(number % 1000));

    getCoreP()->scheduleAction(std::move(newAction));
}

bool VehicleObject::isInitialized()
{
    return BaseObject::isInitialized() && mExternalId != "";
}

void VehicleObject::setExternalId(std::string id)
{
    enforce(!isInitialized(), "VehicleObject: it is not allowed to set ExternalId when initialized");
    mExternalId = id;
}


} // namespace paresis