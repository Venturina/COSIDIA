#include "core/Core.hpp"
#include "core/InitAction.hpp"

namespace cosidia
{

InitAction::InitAction(Duration duration, TimePoint start, ObjectId id, ObjectId generator) :
    Action(duration, start, id, generator)
{
    mInitHandler = std::make_shared<ActionHandlerInit>(shared_from_this());
}

void InitAction::scheduleStartHandler()
{
    getCoreP()->scheduleAction(mInitHandler);
}


} // namespace cosidia
