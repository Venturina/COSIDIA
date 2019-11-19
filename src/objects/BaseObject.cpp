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
                getCoreP()->scheduleAction(makeEndAction(action));
                startExecution(std::move(action));
            }
            break;
        case Action::Kind::END:
            {
                endExecution(action);
                if(mActionManager.isActionAvailable()) {
                    auto next = mActionManager.popNextAction();
                    next->setStartTime(action->getStartTime() + action->getDuration());
                    getCoreP()->scheduleAction(makeEndAction(next));
                    startExecution(std::move(next));
                }
                auto now = getCoreP()->getClock()->getSimTimeNow();
                LOG_F(ERROR, "time: expected: %d, now: %d, value: %d", action->getStartTime().count()/1000, now.count()/1000, (action->getStartTime().count() - now.count()) / 1000);
                if(!((action->getStartTime() < std::chrono::seconds(3)) ||
                    (action->getStartTime() - now) > std::chrono::milliseconds(-5))) {
                    throw std::runtime_error("Time Violation");
                }
                break;
            }
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
    assert(!mParentList.empty());

    return true;
}

std::shared_ptr<Action> BaseObject::createSelfAction(SteadyClock::duration duration, SteadyClock::duration start)
{
    return std::move(std::make_shared<Action>(duration, Action::Kind::START, start, mObjectId));
}

}