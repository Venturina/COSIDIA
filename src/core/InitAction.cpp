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
    getCoreP()->scheduleAction(mInitHandler);
}

void InitAction::afterConstruction()
{
    mInitHandler = std::make_shared<ActionHandlerInit>(shared_from_this());
}

std::shared_ptr<InitAction> InitAction::create(Duration duration, TimePoint start, ObjectId id, ObjectId generator)
{
    auto instance = std::shared_ptr<InitAction>(new InitAction {duration, start, id, generator});
    instance->afterConstruction();
    return instance;
}


} // namespace cosidia
