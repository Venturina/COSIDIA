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
     * Adds an Handler to the ActionList
     * @param Handler to insert
     */
    void insertHandler(HandlerP);

    /**
     * Returns next time point at which an Handler is available
     */
    SimClock::time_point getNextTimePoint();

    /**
     * Remove handlers for next time point from Action list
     * @return next Handlers
     */
    std::list<HandlerP> popNextHandlers();

    /**
     * Returns next handlers without removing it.
     * @return next handlers
     */
    std::list<HandlerP> getNextHandlerList() const;

    /**
     * Removes specific handler from Handler list
     * @param handler to remove
     * @param time at which the handler was scheduled
     * @return true if remove was successfull, false elsewise
     */
    bool removeHandler(ConstHandlerP, SimClock::time_point);

protected:
    std::unique_ptr<HandlerMap> mHandlerMap;
};

} // ns cosidia

#endif /* _ACTION_LIST_HPP_SDNOL */