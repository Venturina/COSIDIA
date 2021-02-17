#pragma once

#include "core/Action.hpp"
#include "core/ActionHandler.hpp"

namespace cosidia
{

class InitAction : public Action {
public:
    /**
     * Schedules StartHandler at the Core
     */
    void scheduleStartHandler();

    /**
     * An Init Action does not have an End handler.
     */
    void scheduleEndHandler() {};

    friend class ActionFactory<InitAction>;

protected:
    /**
     * Used to create the Action using the factory class
     */
    void afterConstruction();

    /**
     * Actions can only be created using the factory class
     */
    InitAction(Duration duration, TimePoint start, ObjectId id, ObjectId generator);
    std::shared_ptr<ActionHandlerInit> mInitHandler;
};

} // namespace cosidia



