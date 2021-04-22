#pragma once

#include "core/Action.hpp"

#include <boost/circular_buffer.hpp>

namespace cosidia
{

class ActionHistory
{
public:
    ActionHistory(int size);

    void addStartedAction(std::shared_ptr<const Action>);
    void addDelayedAction(std::shared_ptr<const Action>);
    void addEndedAction(std::shared_ptr<const Action>);
    void addDequeuedAction(std::shared_ptr<const Action>);
    void addInitAction(std::shared_ptr<const Action>);
    void prettyPrint();
private:
    using ActionState = std::pair<std::shared_ptr<const Action>, std::string>;

    std::unique_ptr<boost::circular_buffer<ActionState>> mBuffer;
};

} // namespace cosidia