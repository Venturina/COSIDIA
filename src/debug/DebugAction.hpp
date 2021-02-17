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

protected:

    void afterConstruction() { DurationAction::afterConstruction(); }

    DebugDurationAction(Duration duration, TimePoint start, ObjectId id, ObjectId generator) :
        DurationAction(duration,start, id, generator) {}
};

class DebugInitAction : public InitAction
{
public:
    std::shared_ptr<ActionHandlerInit> getInitHandler() { return mInitHandler; }

protected:
    static std::shared_ptr<DebugInitAction> create(Duration duration, TimePoint start, ObjectId id, ObjectId generator)
        { return create(duration, start, id, generator); }

    void afterConstruction() { InitAction::afterConstruction(); }

    DebugInitAction(Duration duration, TimePoint start, ObjectId id, ObjectId generator) :
        InitAction(duration,start, id, generator) {}
};



} // namespace debug
} // namespace cosidia
