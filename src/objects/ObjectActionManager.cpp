#include "objects/ObjectActionManager.hpp"

#include "loguru/loguru.hpp"

namespace cosidia
{

bool ObjectActionManager::isActionRunning()
{
    return mActiveAction.get() != nullptr;
}

void ObjectActionManager::setActiveAction(std::shared_ptr<Action> action)
{
    mActiveAction = action;
}

bool ObjectActionManager::startOrDelay(std::shared_ptr<Action> action)
{
    if (mActiveAction.get() == nullptr && mDelayedQueue.size() == 0) {
        mActiveAction = action;
        return false;
    } else {
        mDelayedQueue.push(action);
        DLOG_F(INFO, "Action running, queue other action. If this happens often, think about your model");
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


} // namespace cosidia
