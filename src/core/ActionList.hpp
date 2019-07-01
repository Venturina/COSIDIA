#ifndef _ACTION_LIST_HPP_SDNOL
#define _ACTION_LIST_HPP_SDNOL

#include "core/Action.hpp"
#include <map>
#include <memory>


namespace paresis
{
namespace core
{

typedef std::map<uint64_t, std::shared_ptr<Action>> ActionMap;
typedef std::shared_ptr<Action> ActionP;
typedef std::shared_ptr<const Action> ConstActionP;

class ActionList
{
    void insertTask(ActionP);
    ActionP popNextTask();
    ConstActionP getNextTask() const;


private:
    ActionMap mActionMap;
};

} // ns core
} // ns paresis

#endif /* _ACTION_LIST_HPP_SDNOL */