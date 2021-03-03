#include "core/ActionHandler.hpp"
#include "objects/BaseObject.hpp"

namespace cosidia
{

void ActionHandlerStart::invoke(BaseObject* base)
{
    base->startExecutionBase(mCorrespondingAction);
}

SimClock::time_point ActionHandlerStart::getTime()
{
    return mCorrespondingAction->getStartTime();
}

void ActionHandlerEnd::invoke(BaseObject* base)
{
    base->endExecutionBase(mCorrespondingAction);
}

SimClock::time_point ActionHandlerEnd::getTime()
{
    return mCorrespondingAction->getEndTime();
}

void ActionHandlerInit::invoke(BaseObject* base)
{
    base->initObjectBase(mCorrespondingAction);
}

SimClock::time_point ActionHandlerInit::getTime()
{
    return mCorrespondingAction->getStartTime();
}


} // namespace cosidia
