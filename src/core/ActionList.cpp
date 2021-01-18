#include "core/ActionList.hpp"
#include "core/Core.hpp"
#include "utils/enforce.hpp"

namespace cosidia
{

ActionList::ActionList()
{
    mActionMap = std::make_unique<ActionMap>();
}

void ActionList::insertAction(ActionP action)
{
    enforce(onCoreThread(), "Inserted Action from wrong thread!");
    mActionMap->insert(std::pair<SimClock::time_point, ActionP>(action->getStartTime(), action));
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

bool ActionList::removeAction(ConstActionP actionToRemove, SimClock::time_point removeTimeHint)
{
    auto possibleRemoveElements = mActionMap->equal_range(removeTimeHint);
    for(auto i = possibleRemoveElements.first; i != possibleRemoveElements.second; i++) {
        if(i->second == actionToRemove) {
            mActionMap->erase(i);
            return true;
            break;
        }
    }
    return false;
}

} // ns cosidia