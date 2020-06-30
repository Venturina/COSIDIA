#include "core/Core.hpp"
#include "objects/BaseObject.hpp"
#include "utils/invariant.hpp"

#include <loguru/loguru.hpp>

namespace paresis
{

BaseObject::BaseObject() : mActionManager(new ObjectActionManager())
{
}

BaseObject::~BaseObject()
{
    DLOG_F(ERROR, "Called Destructor of %d, %s", mObjectId, mObjectName.c_str());
}

void BaseObject::execute(std::shared_ptr<Action> action)
{
    invariant(isInitialized(), "Tried to execute a not initialized object");

    switch (action->getKind()) {
        case Action::Kind::START:
            if(!mActionManager->startOrDelay(action)) {
                getCoreP()->scheduleAction(makeEndAction(action));
                startExecution(std::move(action));
            }
            break;
        case Action::Kind::END:
            {
                endExecution(action);
                auto now = getCoreP()->getClock()->getSimTimeNow();
                LOG_F(ERROR, "time: expected: %d now: %d value: %d, type %s", action->getStartTime().count()/1000, now.count()/1000, (action->getStartTime().count() - now.count()) / 1000, mObjectName.c_str());
                if(!((action->getStartTime() < std::chrono::seconds(3)) ||
                    (action->getStartTime() - now) > std::chrono::milliseconds(-1))) {
                    throw std::runtime_error("Time Violation");
                }
                if(mActionManager->endAndCheckAvailable()) {
                    auto update = mActionManager->fetchNextAction();
                    update.setStartTime(action->getStartTime() + action->getDuration());
                    auto next = mActionManager->activateNextAvailableAction();
                    getCoreP()->scheduleAction(makeEndAction(next));

                    startExecution(std::move(next));
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
    if(mObjectName.compare("") == 0 || mObjectId == -1 || mParentList.empty()) {
        return false;
    } else {
        return true;
    }
}

std::shared_ptr<Action> BaseObject::createSelfAction(SteadyClock::duration duration, SteadyClock::duration start)
{
    return std::move(std::make_shared<Action>(duration, Action::Kind::START, start, mObjectId));
}

}