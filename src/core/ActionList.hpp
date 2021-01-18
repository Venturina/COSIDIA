#ifndef _ACTION_LIST_HPP_SDNOL
#define _ACTION_LIST_HPP_SDNOL

#include "core/Action.hpp"
#include <chrono>
#include <map>
#include <memory>


namespace cosidia
{

using ActionP = std::shared_ptr<Action>;
using ConstActionP = std::shared_ptr<const Action>;
using ActionMap = std::multimap<SimClock::time_point, ActionP>;

/**
 * ActionList is the Future Action Set of the Discrete Action Simulation
 */
class ActionList
{
public:
    /**
     * Creates the ActionList
     */
    ActionList();

    /**
     * Adds an Action to the ActionList
     * @param Action to insert
     */
    void insertAction(ActionP);

    /**
     * Removes the next Action from the ActionList
     * @return next Action
     */
    ActionP popNextAction();

    /**
     * Returns next Action without removing it.
     * @return next Action
     */
    ConstActionP getNextAction() const;

    /**
     * Removes specific Action from Action list
     * @param action to remove
     * @param time at which the action was scheduled
     * @return true if remove was successfull, false elsewise
     */
    bool removeAction(ConstActionP, SimClock::time_point);

protected:
    std::unique_ptr<ActionMap> mActionMap;
};

} // ns cosidia

#endif /* _ACTION_LIST_HPP_SDNOL */