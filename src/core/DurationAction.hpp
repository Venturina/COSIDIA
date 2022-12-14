#pragma once

#include "core/Action.hpp"
#include "core/ActionHandler.hpp"

namespace cosidia
{

/**
 * An DurationAction is an Action with an start and an end handler
 */
class DurationAction : public Action {
public:
    /**
     * Schedule the handlers at the core
     */
    void scheduleStartHandler();
    void scheduleEndHandler();

    friend class ActionFactory<DurationAction>;

protected:
    /**
     * Used to create the Action using the factory class
     */
    void afterConstruction();

    /**
     * Actions can only be created using the factory class
     */
    DurationAction(Duration duration, TimePoint start, ObjectId id, ObjectId generator);

    std::shared_ptr<ActionHandlerStart> mStartHandler;
    std::shared_ptr<ActionHandlerEnd> mEndHandler;
};

} // namespace cosidia



