#include "core/Core.hpp"
#include "objects/VehicleObject.hpp"

#include <loguru/loguru.hpp>

namespace paresis
{

VehicleObject::VehicleObject(Core* c) : BaseObject(c)
{

}

void VehicleObject::startExecution(std::shared_ptr<Action> action)
{
    DLOG_F(WARNING, "VehicleObject %d, executed at %d", mObjectId ,std::chrono::duration_cast<std::chrono::milliseconds>(action->getStartTime()).count());

    auto newAction = createSelfAction(std::chrono::milliseconds(50), action->getStartTime() + std::chrono::milliseconds(100));
    mCore->scheduleAction(newAction);
}

void VehicleObject::endExecution(std::shared_ptr<Action> action)
{

}


void VehicleObject::initObject(std::shared_ptr<Action> action)
{
    auto number = mCore->getRandomNumber();
    DLOG_F(WARNING, "VehicleObject Init with rnd: %d", number);
    // random number to avoid vehicle synchronisation
    auto newAction = createSelfAction(std::chrono::milliseconds(50), action->getStartTime() + std::chrono::milliseconds(number % 1000));

    mCore->scheduleAction(std::move(newAction));
}


} // namespace paresis


