#include "core/Core.hpp"
#include "core/DurationAction.hpp"

namespace cosidia
{

DurationAction::DurationAction(Duration duration, TimePoint start, ObjectId id, ObjectId generator) :
    Action(duration, start, id, generator)
{
}

void DurationAction::scheduleStartHandler()
{
    getCoreP()->scheduleAction(mStartHandler);
}

void DurationAction::scheduleEndHandler()
{
    getCoreP()->scheduleAction(mEndHandler);
}

void DurationAction::afterConstruction()
{
    mStartHandler = std::make_shared<ActionHandlerStart>(shared_from_this());
    mEndHandler = std::make_shared<ActionHandlerEnd>(shared_from_this());
}

std::shared_ptr<DurationAction> DurationAction::create(Duration duration, TimePoint start, ObjectId id, ObjectId generator)
{
    auto instance = std::shared_ptr<DurationAction>(new DurationAction {duration, start, id, generator});
    instance->afterConstruction();
    return instance;
}


} // namespace cosidia
