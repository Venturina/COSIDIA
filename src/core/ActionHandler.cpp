#include "core/ActionHandler.hpp"
#include "objects/BaseObject.hpp"

namespace cosidia
{

void ActionHandlerStart::invoke(BaseObject* base)
{
    base->startExecutionBase(mCorrespondingAction);
}

void ActionHandlerEnd::invoke(BaseObject* base)
{
    base->endExecutionBase(mCorrespondingAction);
}

void ActionHandlerInit::invoke(BaseObject* base)
{
    base->initObjectBase(mCorrespondingAction);
}

} // namespace cosidia
