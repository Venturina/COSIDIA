#pragma once

#include "core/Action.hpp"
#include "core/ActionHandler.hpp"

namespace cosidia
{

class InitAction : public Action {
public:
    void scheduleStartHandler();

    void scheduleEndHandler() {};


protected:
    static std::shared_ptr<InitAction> create(Duration duration, TimePoint start, ObjectId id, ObjectId generator);
    void afterConstruction();

private:
    InitAction(Duration duration, TimePoint start, ObjectId id, ObjectId generator);
    std::shared_ptr<ActionHandlerInit> mInitHandler;
};

} // namespace cosidia



