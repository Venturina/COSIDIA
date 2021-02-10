#pragma once

#include "core/Action.hpp"
#include "core/ActionHandler.hpp"

namespace cosidia
{

class InitAction : public Action {
public:
    InitAction(Duration duration, TimePoint start, ObjectId id, ObjectId generator);

    void scheduleStartHandler();

    void scheduleEndHandler() {};

private:
    void setup();
    std::shared_ptr<ActionHandlerInit> mInitHandler;
};

} // namespace cosidia



