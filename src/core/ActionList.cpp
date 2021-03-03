#include "core/ActionList.hpp"
#include "core/Core.hpp"
#include "utils/enforce.hpp"

namespace cosidia
{

ActionList::ActionList()
{
    mHandlerMap = std::make_unique<HandlerMap>();
}

SimClock::time_point ActionList::getNextTimePoint()
{
    if(mHandlerMap->empty()) {
        return SimClock::invalid();
    } else {
        return mHandlerMap->begin()->first;
    }
}

void ActionList::insertHandler(HandlerP handler)
{
    enforce(onCoreThread(), "Inserted Action from wrong thread!");
    (*mHandlerMap)[handler->getTime()].push_back(handler);
}

std::list<HandlerP> ActionList::popNextHandlers()
{
    enforce(onCoreThread(), "Pop Action from wrong thread!");
    if(!mHandlerMap->empty()) {
        enforce(!mHandlerMap->begin()->second.empty(), "ActionList: Time point with empty list");
        auto nextActionIt = mHandlerMap->begin();
        auto nextAction = nextActionIt->second;
        mHandlerMap->erase(nextActionIt);
        return nextAction;
    } else {
        return std::list<HandlerP>{};
    }
}

std::list<HandlerP> ActionList::getNextHandlerList() const
{
    enforce(onCoreThread(), "Get Action from wrong thread!");
    if(!mHandlerMap->empty()) {
        enforce(!mHandlerMap->begin()->second.empty(), "ActionList: Time point with empty list");
        return mHandlerMap->begin()->second;
    } else {
        return std::list<HandlerP>{};
    }
}

//TODO: Removing handlers is not allowed yet
bool ActionList::removeHandler(ConstHandlerP actionToRemove, SimClock::time_point removeTimeHint)
{
    enforce(false, "ActionList: removing of Handlers is not allowed, yet");
}

} // ns cosidia