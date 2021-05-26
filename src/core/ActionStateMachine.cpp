#include "core/ActionStateMachine.hpp"
#include "core/Action.hpp"
#include "utils/enforce.hpp"

namespace cosidia
{

void ActionStateMachine::setRunning()
{
    if(mState != STATE::WAITING) {
        mAction->prettyPrint();
        mState = STATE::FAILURE;
        throw std::runtime_error("ActionStateMachine: Wrong State for begin Action");
    } else {
        mState = STATE::RUNNING;
    }
}

void ActionStateMachine::setFinished()
{
    if(mState != STATE::RUNNING) {
        mAction->prettyPrint();
        mState = STATE::FAILURE;
        throw std::runtime_error("ActionStateMachine: Wrong State for end Action");
    } else {
        mState = STATE::FINISHED;
    }
}

} // namespace cosidia