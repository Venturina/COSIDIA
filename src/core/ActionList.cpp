#include "core/ActionList.hpp"


namespace paresis
{

bool compareFunction(uint64_t lhs, uint64_t rhs)
{
    return lhs < rhs;
}

ActionList::ActionList()
{
    mActionMap = std::make_unique<ActionMap>(ActionMap(compareFunction));
}

void ActionList::insertAction(ActionP action)
{
    mActionMap->insert(std::pair<uint64_t, ActionP>(action->getStartTime(), action));
}

ActionP ActionList::popNextAction()
{
    if(!mActionMap->empty()) {
        auto nextActionIt = mActionMap->begin();
        auto nextAction = nextActionIt->second;
        mActionMap->erase(nextActionIt);
        return nextAction;
    } else {
        return nullptr;
    }
}

ConstActionP ActionList::getNextAction() const
{
    if(!mActionMap->empty()) {
        return mActionMap->begin()->second;
    } else {
        return nullptr;
    }
}

} // ns paresis