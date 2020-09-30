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

class ActionList
{
public:
    ActionList();
    void insertAction(ActionP);
    ActionP popNextAction();
    ActionP getNextAction() const;


private:
    std::unique_ptr<ActionMap> mActionMap;
};

} // ns paresis

#endif /* _ACTION_LIST_HPP_SDNOL */