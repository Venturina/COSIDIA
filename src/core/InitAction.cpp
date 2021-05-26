#include "core/Core.hpp"
#include "core/InitAction.hpp"
#include "utils/enforce.hpp"

namespace cosidia
{

InitAction::InitAction(Duration duration, TimePoint start, ObjectId id, ObjectId generator) :
    Action(duration, start, id, generator)
{
}

void InitAction::scheduleStartHandler()
{
    enforce(mActionId == 0, "InitAction: tried to schedule Action with Id already set");
    #ifdef COSIDIA_SAFE
    mStateMachine.setRunning();
    mStateMachine.setFinished();
    #endif
    getCoreP()->scheduleAction(mInitHandler);
}

void InitAction::afterConstruction()
{
    mInitHandler = std::make_shared<ActionHandlerInit>(shared_from_this());
}

} // namespace cosidia
