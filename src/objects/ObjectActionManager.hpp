#ifndef OBJECT_ACTION_MANAGER_HPP
#define OBJECT_ACTION_MANAGER_HPP

#include "core/Action.hpp"
#include <queue>

namespace paresis
{

/**
 * This class manages the Actions of each object to avoid concurrent execution of one object
 **/

class ObjectActionManager
{
public:
    bool isActionRunning();
    void setActiveAction(std::shared_ptr<Action>);

    /**
     * Checks if a Action can be started immediately
     * In case no other action is running, false is returned and action is stored as active action
     * In case an action is running, the passed action will be delayed and true is returned
     * @param: Action to be scheduled
     * @return: true if action is allready running, false if no action is running
     **/
    bool startOrDelay(std::shared_ptr<Action>);

    bool isActionAvailable();

    std::shared_ptr<Action> popNextAction();

private:
    std::shared_ptr<Action> mActiveAction;
    std::queue<std::shared_ptr<Action>> mDelayedQueue;
};

} // namespace paresis

#endif /* OBJECT_ACTION_MANAGER_HPP */