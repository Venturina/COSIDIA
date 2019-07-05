#ifndef _ACTION_LIST_HPP_SDNOL
#define _ACTION_LIST_HPP_SDNOL

#include "core/Action.hpp"
#include <map>
#include <memory>


namespace paresis
{
namespace core
{

using ActionP = std::shared_ptr<Action>;
using ConstActionP = std::shared_ptr<const Action>;
using ActionMap = std::multimap<uint64_t, ActionP, bool(*)(uint64_t, uint64_t)>;

bool compareFunction(uint64_t lhs, uint64_t rhs);

class ActionList
{
public:
    ActionList();
    void insertAction(ActionP);
    ActionP popNextAction();
    ConstActionP getNextAction() const;


private:
    std::unique_ptr<ActionMap> mActionMap;
};

} // ns core
} // ns paresis

#endif /* _ACTION_LIST_HPP_SDNOL */