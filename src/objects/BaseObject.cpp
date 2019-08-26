#include "core/Core.hpp"
#include "objects/BaseObject.hpp"

#include <loguru/loguru.hpp>

namespace paresis
{

BaseObject::BaseObject()
{
}

void BaseObject::execute(std::shared_ptr<Action> action)
{
    assert(isInitialized());

    switch (action->getKind()) {
        case Action::Kind::START:
            if(!mActionManager.startOrDelay(action)) {
                startExecution(std::move(action));
            }
            break;
        case Action::Kind::END:
            endExecution(std::move(action));
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

bool BaseObject::isInitialized()
{
    assert(mObjectName.compare(""));
    assert(mObjectId != -1);
    assert(mParent != -1);
    
    return true;
}

std::shared_ptr<Action> BaseObject::createSelfAction(SteadyClock::duration duration, SteadyClock::duration start)
{
    return std::move(std::make_shared<Action>(duration, Action::Kind::START, start, mObjectId));
}

}