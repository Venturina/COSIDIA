#pragma once

#include "core/SteadyClock.hpp"
#include <memory>

namespace cosidia
{

class Action;
class BaseObject;

/**
 * The ActionHandler class is used to schedule start and end time points of an action.
 * The core holds a list of handlers which are invoked at their corresponding time
 */
class ActionHandler {
public:
    using TimePoint = SimClock::time_point;

    ActionHandler(std::shared_ptr<Action> action) : mCorrespondingAction(action) {};

    virtual void invoke(BaseObject*) = 0;

    Action* getAction() { return mCorrespondingAction.get(); }

    virtual TimePoint getTime() = 0;

protected:
    std::shared_ptr<Action> mCorrespondingAction;
};

/**
 * The ActionHandlerStart class invokes the "startExecutionBase()" method of the BaseObject
 */
class ActionHandlerStart : public ActionHandler {
public:
    ActionHandlerStart(std::shared_ptr<Action> action) : ActionHandler(action) {}
    void invoke(BaseObject*);
    TimePoint getTime();
};

/**
 * The ActionHandlerEnd class invokes the "endExecutionBase()" method of the BaseObject
 */
class ActionHandlerEnd : public ActionHandler {
public:
    ActionHandlerEnd(std::shared_ptr<Action> action) : ActionHandler(action) {}
    void invoke(BaseObject*);
    TimePoint getTime();
};

/**
 * The ActionHandlerInit class invokes the "initBase()" method of the BaseObject
 */
class ActionHandlerInit : public ActionHandler {
public:
    ActionHandlerInit(std::shared_ptr<Action> action) : ActionHandler(action) {}
    void invoke(BaseObject*);
    TimePoint getTime();
};

} // namespace cosidia
