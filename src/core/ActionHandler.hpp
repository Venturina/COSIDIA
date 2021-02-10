#pragma once

#include <memory>

namespace cosidia
{

class Action;
class BaseObject;

class ActionHandler {
public:
    ActionHandler(std::shared_ptr<Action> action) : mCorrespondingAction(action) {};

    virtual void invoke(BaseObject*) = 0;

    Action* getAction() { return mCorrespondingAction.get(); }

protected:
    std::shared_ptr<Action> mCorrespondingAction;
};

class ActionHandlerStart : public ActionHandler {
public:
    ActionHandlerStart(std::shared_ptr<Action> action) : ActionHandler(action) {}
    void invoke(BaseObject*);
};

class ActionHandlerEnd : public ActionHandler {
public:
    ActionHandlerEnd(std::shared_ptr<Action> action) : ActionHandler(action) {}
    void invoke(BaseObject*);
};

class ActionHandlerInit : public ActionHandler {
public:
    ActionHandlerInit(std::shared_ptr<Action> action) : ActionHandler(action) {}
    void invoke(BaseObject*);
};

} // namespace cosidia
