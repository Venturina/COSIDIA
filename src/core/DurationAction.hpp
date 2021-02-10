#pragma once

#include "core/Action.hpp"
#include "core/ActionHandler.hpp"

namespace cosidia
{

class DurationAction : public Action {
public:
    DurationAction(Duration duration, TimePoint start, ObjectId id, ObjectId generator);

    void scheduleStartHandler();
    void scheduleEndHandler();

private:
    std::shared_ptr<ActionHandlerStart> mStartHandler;
    std::shared_ptr<ActionHandlerEnd> mEndHandler;
};

} // namespace cosidia



