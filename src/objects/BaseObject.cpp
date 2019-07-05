#include "objects/BaseObject.hpp"

namespace paresis
{

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
            initObject(action);
            break;
        default:
            break;
    }
}

}