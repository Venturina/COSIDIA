#pragma once

#include "core/Action.hpp"
#include "core/ActionHandler.hpp"

namespace cosidia
{

class DurationAction : public Action {
public:
    void scheduleStartHandler();
    void scheduleEndHandler();

protected:
    static std::shared_ptr<DurationAction> create(Duration duration, TimePoint start, ObjectId id, ObjectId generator);
    void afterConstruction();

private:
    DurationAction(Duration duration, TimePoint start, ObjectId id, ObjectId generator);
    std::shared_ptr<ActionHandlerStart> mStartHandler;
    std::shared_ptr<ActionHandlerEnd> mEndHandler;
};

} // namespace cosidia



