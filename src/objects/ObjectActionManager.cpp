#include "objects/ObjectActionManager.hpp"

namespace paresis
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
    if (mActiveAction.get() == nullptr) {
        return false;
    } else {
        mDelayedQueue.push(action);
        return true;
    }
}

bool ObjectActionManager::isActionAvailable()
{
    return mDelayedQueue.size() != 0;
}

std::shared_ptr<Action> ObjectActionManager::popNextAction()
{
    auto action = mDelayedQueue.front();
    mDelayedQueue.pop();
    return action;
}

} // namespace paresis
