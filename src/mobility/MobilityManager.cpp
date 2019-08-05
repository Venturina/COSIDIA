#include "core/Core.hpp"
#include "mobility/MobilityManager.hpp"

namespace paresis
{

void MobilityManager::startExecution(std::shared_ptr<Action>)
{

}

void MobilityManager::endExecution(std::shared_ptr<Action> action)
{
    auto newAction = std::make_shared<Action>(std::chrono::milliseconds(50),
        Action::Kind::START, action->getStartTime() + std::chrono::milliseconds(100),
        (*mCore->getCurrentObjectList())[mObjectId]); // ugly as hell?

    mCore->scheduleAction(newAction);

}

void MobilityManager::initObject(std::shared_ptr<Action> action)
{
    std::stringstream s;
    s << "Id of MobilityManager: " << mObjectId << std::endl;
    std::cout << s.str() << std::endl;

    auto newAction = std::make_shared<Action>(std::chrono::milliseconds(50),
        Action::Kind::START, action->getStartTime() + std::chrono::milliseconds(100),
        (*mCore->getCurrentObjectList())[mObjectId]); // ugly as hell?

    mCore->scheduleAction(newAction);

}

} // namespace paresis
