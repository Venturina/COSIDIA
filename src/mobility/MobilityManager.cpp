#include "core/Core.hpp"
#include "mobility/MobilityManager.hpp"

#include <loguru/loguru.hpp>

namespace paresis
{

void MobilityManager::startExecution(std::shared_ptr<Action>)
{
    DLOG_F(INFO, "MobilityManager update");

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
    DLOG_F(INFO, "Id of MobilityManager: %d", mObjectId);

    auto newAction = std::make_shared<Action>(std::chrono::milliseconds(50),
        Action::Kind::START, action->getStartTime() + std::chrono::milliseconds(100),
        (*mCore->getCurrentObjectList())[mObjectId]); // ugly as hell?

    mCore->scheduleAction(newAction);

}

} // namespace paresis
