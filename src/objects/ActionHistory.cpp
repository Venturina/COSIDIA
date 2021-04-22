#include "objects/ActionHistory.hpp"

namespace cosidia
{

ActionHistory::ActionHistory(int size) : mBuffer(new boost::circular_buffer<ActionState>(size))
{
}

void ActionHistory::addStartedAction(std::shared_ptr<const Action> action)
{
    mBuffer->push_front(std::make_pair(action, "started"));
}

void ActionHistory::addEndedAction(std::shared_ptr<const Action> action)
{
    mBuffer->push_front(std::make_pair(action, "ended"));
}

void ActionHistory::addDelayedAction(std::shared_ptr<const Action> action)
{
    mBuffer->push_front(std::make_pair(action, "delayed"));
}

void ActionHistory::addDequeuedAction(std::shared_ptr<const Action> action)
{
    mBuffer->push_front(std::make_pair(action, "dequeued"));
}

void ActionHistory::addInitAction(std::shared_ptr<const Action> action)
{
    mBuffer->push_front(std::make_pair(action, "init"));
}

void ActionHistory::prettyPrint()
{
    auto it = mBuffer->begin();
    auto end = mBuffer->end();
    for(; it != end; it++) {
        auto pair = *it;
        auto action = pair.first;
        auto state = pair.second;

        std::cout << "Action Id: " << action->getActionId() << " | state: " << state << " | Destination: " << action->getAffected().raw() << " | Start time: " << action->getStartTime().time_since_epoch().count() / 1000 <<
            " | Originating Object: " << action->getGeneratingObject().raw() << " | Type: " << action->getType().value() << std::endl;
    }
}

} // namespace cosidia
