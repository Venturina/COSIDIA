#include "objects/ObjectActionManager.hpp"

#include "loguru/loguru.hpp"

namespace cosidia
{

bool ObjectActionManager::isActionRunning()
{
    return mActiveAction.get() != nullptr;
}

bool ObjectActionManager::startOrDelay(std::shared_ptr<Action> action)
{
    if (mActiveAction.get() == nullptr && mDelayedQueue.size() == 0) {
        mActiveAction = action;
        return false;
    } else {
        mDelayedQueue.push(action);
        DLOG_F(INFO, "Action %d, '%s' delayed on Object %d at time %d. Action %d, '%s' running. If this happens often, think about your model", action->getActionId(), symbol_as_string(action->getType()).c_str(), mCorrespondingObject.raw(), SimClock::getMilliseconds(action->getStartTime()), mActiveAction->getActionId(), symbol_as_string(mActiveAction->getType()).c_str());
        return true;
    }
}

bool ObjectActionManager::isActionAvailable()
{
    return mDelayedQueue.size() != 0;
}

bool ObjectActionManager::endAndCheckAvailable()
{
    mActiveAction = nullptr;
    return isActionAvailable();
}

Action& ObjectActionManager::fetchNextAction()
{
    assert(mDelayedQueue.size() > 0);
    auto action = mDelayedQueue.front();
    return *action;
}

std::shared_ptr<Action> ObjectActionManager::activateNextAvailableAction()
{
    assert(mDelayedQueue.size() > 0);
    if(mActiveAction != nullptr) {
        throw std::runtime_error("Tried to activate action, but other action already running");
    }
    mActiveAction = mDelayedQueue.front();

    mDelayedQueue.pop();
    return mActiveAction;
}

void ObjectActionManager::endManually() {
    mActiveAction = nullptr;
}

void ObjectActionManager::prettyPrint()
{
    std::cout << " Current active action: ";
    mActiveAction->prettyPrint();

    std::cout << "Waiting Actions: ";
    std::queue temp = mDelayedQueue;

    while(!temp.empty()) {
        auto elem = temp.front();
        elem->prettyPrint();
        temp.pop();
    }

    std::cout << "ObjectActionManager done printing" << std::endl;
}


} // namespace cosidia
