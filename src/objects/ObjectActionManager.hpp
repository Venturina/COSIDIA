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


    /**
     * Checks if a Action can be started immediately
     * In case no other action is running, false is returned and action is stored as active action
     * In case an action is running, the passed action will be delayed and true is returned
     * @param: Action to be scheduled
     * @return: true if action is allready running, false if no action is running
     **/
    virtual bool startOrDelay(std::shared_ptr<Action>);

    /**
     * Checks if a Action is available on the action stack
     * @return: true if a action is available, false if not
     **/
    bool isActionAvailable();

    /**
     * Fetches first action and allows to modify its content, action stays in the queue
     * @return: Next action to be scheduled
     **/
    Action& fetchNextAction();

    /**
     * Removes action from queue and sets it to active
     * @return: action which was set to active
     * @return: nullpointer in case action could not be set active eg. because other action was active or no action in queue
     **/
    std::shared_ptr<Action> activateNextAvailableAction();

    /**
     * Ends the current active action and checks if there is a next one to schedule
     * @return: true if a action is available, false if not
     **/
    bool endAndCheckAvailable();

private:
    bool isActionRunning();
    void setActiveAction(std::shared_ptr<Action>);

    std::shared_ptr<Action> mActiveAction;
    std::queue<std::shared_ptr<Action>> mDelayedQueue;
};

} // namespace paresis

#endif /* OBJECT_ACTION_MANAGER_HPP */