#include "objects/BaseObject.hpp"

namespace paresis
{
namespace object
{

void BaseObject::execute(std::shared_ptr<core::Action> action)
{
    switch (action->getKind()) {
        case core::Action::Kind::START:
            startExecution(action);
            break;
        case core::Action::Kind::END:
            endExecution(action);
            break;
        case core::Action::Kind::INIT:
            initObject(action);
            break;
        default:
            break;
    }
}

}
}