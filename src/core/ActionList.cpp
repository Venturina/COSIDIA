#include "core/ActionList.hpp"
#include "core/Core.hpp"
#include "utils/enforce.hpp"

namespace cosidia
{

ActionList::ActionList()
{
    mActionMap = std::make_unique<HandlerMap>();
}

SimClock::time_point ActionList::getNextTimePoint()
{
    if(mActionMap->empty()) {
        return SimClock::invalid();
    } else {
        return mActionMap->begin()->first;
    }
}

void ActionList::insertAction(HandlerP handler)
{
    enforce(onCoreThread(), "Inserted Action from wrong thread!");
    (*mActionMap)[handler->getAction()->getStartTime()].push_back(handler);
}

std::list<HandlerP> ActionList::popNextActions()
{
    enforce(onCoreThread(), "Pop Action from wrong thread!");
    if(!mActionMap->empty()) {
        enforce(!mActionMap->begin()->second.empty(), "ActionList: Time point with empty list");
        auto nextActionIt = mActionMap->begin();
        auto nextAction = nextActionIt->second;
        mActionMap->erase(nextActionIt);
        return nextAction;
    } else {
        return std::list<HandlerP>{};
    }
}

std::list<HandlerP> ActionList::getNextActionList() const
{
    enforce(onCoreThread(), "Get Action from wrong thread!");
    if(!mActionMap->empty()) {
        enforce(!mActionMap->begin()->second.empty(), "ActionList: Time point with empty list");
        return mActionMap->begin()->second;
    } else {
        return std::list<HandlerP>{};
    }
}

bool ActionList::removeAction(ConstHandlerP actionToRemove, SimClock::time_point removeTimeHint)
{
    auto actionTimePair = mActionMap->find(removeTimeHint);
    if(actionTimePair != mActionMap->end()) {
        enforce(!actionTimePair->second.empty(), "ActionList: tried to remove Action at timePoint with empty list");
        int size = actionTimePair->second.size();

  //      actionTimePair->second.remove_if( [ actionToRemove ]( ConstHandlerP p ){ return actionToRemove == p; } );
        auto newSize = actionTimePair->second.size();

        if(size > newSize) {
            if(newSize == 0) {
                mActionMap->erase(removeTimeHint);
            }
            return true;
        }
        return false;
    }
    return false;
}

} // ns cosidia