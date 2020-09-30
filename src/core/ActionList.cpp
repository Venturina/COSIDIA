#include "core/ActionList.hpp"
#include "core/Core.hpp"
#include "utils/enforce.hpp"

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
    enforce(onCoreThread(), "Inserted Action from wrong thread!");
    mActionMap->insert(std::pair<std::chrono::nanoseconds, ActionP>(action->getStartTime(), action));
}

ActionP ActionList::popNextAction()
{
    enforce(onCoreThread(), "Pop Action from wrong thread!");
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
    enforce(onCoreThread(), "Get Action from wrong thread!");
    if(!mActionMap->empty()) {
        return mActionMap->begin()->second;
    } else {
        return nullptr;
    }
}

} // ns paresis