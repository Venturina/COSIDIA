#include "core/ActionList.hpp"


namespace paresis
{

bool compareFunction(std::chrono::nanoseconds lhs, std::chrono::nanoseconds rhs)
{
    return lhs < rhs;
}

ActionList::ActionList()
{
    mActionMap = std::make_unique<ActionMap>(ActionMap(compareFunction));
}

void ActionList::insertAction(ActionP action)
{
    mActionMap->insert(std::pair<std::chrono::nanoseconds, ActionP>(action->getStartTime(), action));
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

ActionP ActionList::getNextAction() const
{
    if(!mActionMap->empty()) {
        return mActionMap->begin()->second;
    } else {
        return nullptr;
    }
}

} // ns paresis