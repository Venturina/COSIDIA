#include "objects/ObjectActionManager.hpp"

#include "loguru/loguru.hpp"

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
        mActiveAction = action;
        return false;
    } else {
        mDelayedQueue.push(action);
        LOG_F(INFO, "Action running, queue other action. If this happens often, think about your model");
        return true;
    }
}

bool ObjectActionManager::isActionAvailable()
{
    mActiveAction = nullptr;
    return mDelayedQueue.size() != 0;
}

std::shared_ptr<Action> ObjectActionManager::popNextAction()
{
    auto action = mDelayedQueue.front();
    mDelayedQueue.pop();
    return action;
}

} // namespace paresis
