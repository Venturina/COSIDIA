#include "core/Core.hpp"
#include "objects/BaseObject.hpp"

namespace paresis
{

BaseObject::BaseObject(Core* c) : mCore(c)
{
    mObjectId = mCore->getNextObjectId();
}

void BaseObject::execute(std::shared_ptr<Action> action)
{
    switch (action->getKind()) {
        case Action::Kind::START:
            if(!mActionManager.startOrDelay(action)) {
                startExecution(action);
            }
            break;
        case Action::Kind::END:
            endExecution(action);
            if(mActionManager.isActionAvailable()) {
                startExecution(mActionManager.popNextAction());
            }
            break;
        case Action::Kind::INIT:
            initObject(action);
            break;
        default:
            break;
    }
}

}