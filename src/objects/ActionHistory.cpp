#include "objects/ActionHistory.hpp"

namespace cosidia
{

ActionHistory::ActionHistory(int size) : mBuffer(new boost::circular_buffer<std::shared_ptr<const Action>>(size))
{
}

void ActionHistory::addAction(std::shared_ptr<const Action> action)
{
    mBuffer->push_front(action);
}

void ActionHistory::prettyPrint()
{
    auto it = mBuffer->begin();
    auto end = mBuffer->end();
    for(; it != end; it++) {
        auto action = *it;
        std::cout << "Action Id: " << action->getActionId() << " | Destination: " << action->getAffected().raw() << " | Start time: " << action->getStartTime().time_since_epoch().count() / 1000 <<
            " | Originating Object: " << action->getGeneratingObject().raw() << " | Type: " << action->getType().value() << std::endl;
    }
}

} // namespace cosidia
