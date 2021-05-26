#include "core/Core.hpp"
#include "core/DurationAction.hpp"
#include "utils/enforce.hpp"

namespace cosidia
{

DurationAction::DurationAction(Duration duration, TimePoint start, ObjectId id, ObjectId generator) :
    Action(duration, start, id, generator)
{
}

void DurationAction::scheduleStartHandler()
{
    enforce(mActionId == 0, "DurationAction: tried to schedule Action with Id already set");
    #ifdef COSIDIA_SAFE
    mStateMachine.setRunning();
    #endif
    getCoreP()->scheduleAction(mStartHandler);
}

void DurationAction::scheduleEndHandler()
{
    enforce(mActionId != 0, "DurationAction: tried to schedule endHandler without beginHandler scheduled");
    #ifndef COSIDIA_SAFE
    mStateMachine.setFinished();
    #endif
    getCoreP()->scheduleAction(mEndHandler);
}

void DurationAction::afterConstruction()
{
    mStartHandler = std::make_shared<ActionHandlerStart>(shared_from_this());
    mEndHandler = std::make_shared<ActionHandlerEnd>(shared_from_this());
}

} // namespace cosidia
