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
            startExecution(action);
            break;
        case Action::Kind::END:
            endExecution(action);
            break;
        case Action::Kind::INIT:
            std::cout << "init ssssssssssssssssssssssssssssssssssssss" << std::endl;
            initObject(action);
            break;
        default:
            break;
    }
}

}