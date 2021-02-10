#include "core/Core.hpp"
#include "core/DurationAction.hpp"

namespace cosidia
{

DurationAction::DurationAction(Duration duration, TimePoint start, ObjectId id, ObjectId generator) :
    Action(duration, start, id, generator)
{
    mStartHandler = std::make_shared<ActionHandlerStart>(shared_from_this());
    mEndHandler = std::make_shared<ActionHandlerEnd>(shared_from_this());
}

void DurationAction::scheduleStartHandler()
{
    getCoreP()->scheduleAction(mStartHandler);
}

void DurationAction::scheduleEndHandler()
{
    getCoreP()->scheduleAction(mEndHandler);
}


} // namespace cosidia
