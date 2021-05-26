#pragma once

namespace cosidia
{
class Action;

class ActionStateMachine
{
public:

ActionStateMachine(Action* a) : mAction(a) {};
ActionStateMachine() {};

void setRunning();
void setFinished();

void setAction(Action* a) { mAction = a; }

private:
    enum class STATE {
        WAITING,
        RUNNING,
        FINISHED,
        FAILURE
    };

    Action* mAction;
    STATE mState = STATE::WAITING;

};

} // namespace cosidia
