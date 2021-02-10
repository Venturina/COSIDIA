#ifndef _ACTION_LIST_HPP_SDNOL
#define _ACTION_LIST_HPP_SDNOL

#include "core/Action.hpp"
#include "core/ActionHandler.hpp"
#include <chrono>
#include <map>
#include <memory>


namespace cosidia
{

using ActionP = std::shared_ptr<Action>;
using ConstActionP = std::shared_ptr<const Action>;
using HandlerP = std::shared_ptr<ActionHandler>;
using ConstHandlerP = std::shared_ptr<const Action>;
using HandlerMap = std::map<SimClock::time_point, std::list<HandlerP>>;

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
    void insertAction(HandlerP);

    /**
     * Returns next time point at which an Action is available
     */
    SimClock::time_point getNextTimePoint();

    /**
     * Remove actions for next time point fomr Action list
     * @return next Actions
     */
    std::list<HandlerP> popNextActions();

    /**
     * Returns next actions without removing it.
     * @return next actions
     */
    std::list<HandlerP> getNextActionList() const;

    /**
     * Removes specific Action from Action list
     * @param action to remove
     * @param time at which the action was scheduled
     * @return true if remove was successfull, false elsewise
     */
    bool removeAction(ConstHandlerP, SimClock::time_point);

protected:
    std::unique_ptr<HandlerMap> mActionMap;
};

} // ns cosidia

#endif /* _ACTION_LIST_HPP_SDNOL */