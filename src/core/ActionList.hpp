#ifndef _ACTION_LIST_HPP_SDNOL
#define _ACTION_LIST_HPP_SDNOL

#include "core/Action.hpp"
#include "chrono"
#include <map>
#include <memory>


namespace paresis
{

using ActionP = std::shared_ptr<Action>;
using ConstActionP = std::shared_ptr<const Action>;
using ActionMap = std::multimap<std::chrono::nanoseconds, ActionP, bool(*)(std::chrono::nanoseconds, std::chrono::nanoseconds)>;

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

private:
    std::unique_ptr<ActionMap> mActionMap;
};

} // ns paresis

#endif /* _ACTION_LIST_HPP_SDNOL */