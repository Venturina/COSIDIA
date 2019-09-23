#include "core/Core.hpp"
#include "objects/BaseObject.hpp"

#include <loguru/loguru.hpp>

namespace paresis
{

BaseObject::BaseObject()
{
}

void BaseObject::execute(std::shared_ptr<Action> action)
{
    assert(isInitialized());

    switch (action->getKind()) {
        case Action::Kind::START:
            if(!mActionManager.startOrDelay(action)) {
                getCoreP()->scheduleAction(std::make_shared<Action>(std::chrono::nanoseconds{0}, Action::Kind::END, action->getStartTime() + action->getDuration(), *action->getAffected()));
                startExecution(std::move(action));
            }
            break;
        case Action::Kind::END:
            endExecution(action);
            if(mActionManager.isActionAvailable()) {
                auto next = mActionManager.popNextAction();
                next->setStartTime(action->getStartTime() + action->getDuration());
                getCoreP()->scheduleAction(std::make_shared<Action>(std::chrono::nanoseconds{0}, Action::Kind::END, next->getStartTime() + next->getDuration(), *next->getAffected()));
                startExecution(std::move(next));
            }
            break;
        case Action::Kind::INIT:
            initObject(action);
            break;
        default:
            break;
    }
}

bool BaseObject::isInitialized()
{
    assert(mObjectName.compare(""));
    assert(mObjectId != -1);
    assert(mParent != -1);

    return true;
}

std::shared_ptr<Action> BaseObject::createSelfAction(SteadyClock::duration duration, SteadyClock::duration start)
{
    return std::move(std::make_shared<Action>(duration, Action::Kind::START, start, mObjectId));
}

}