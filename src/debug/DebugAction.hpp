#include "core/DurationAction.hpp"
#include "core/InitAction.hpp"

namespace cosidia
{
namespace debug
{
class DebugDurationAction : public DurationAction
{
public:
    std::shared_ptr<ActionHandlerStart> getStartHandler() { return mStartHandler; }
    std::shared_ptr<ActionHandlerEnd> getEndHandler() { return mEndHandler; }

    friend class ActionFactory<DebugDurationAction>;

protected:
    using DurationAction::DurationAction;
};
class DebugInitAction : public InitAction
{
public:
    std::shared_ptr<ActionHandlerInit> getInitHandler() { return mInitHandler; }

    friend class ActionFactory<DebugInitAction>;

protected:
    using InitAction::InitAction;
};

} // namespace debug
} // namespace cosidia
