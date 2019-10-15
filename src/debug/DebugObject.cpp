#include "core/Core.hpp"
#include "debug/DebugObject.hpp"
#include "loguru/loguru.hpp"

namespace paresis
{
DebugObject::DebugObject() : BaseObject()
{
    mObjectName = "DebugObject";
}

void DebugObject::startExecution(std::shared_ptr<Action>)
{
    DLOG_F(INFO, "Debug Event Start");
}

void DebugObject::endExecution(std::shared_ptr<Action>)
{
    DLOG_F(INFO, "Debug Event End");
}

void DebugObject::initObject(std::shared_ptr<Action>)
{
    auto action = std::make_shared<Action>(std::chrono::seconds{1}, Action::Kind::START, std::chrono::milliseconds{50}, mObjectId);
    auto action2 = std::make_shared<Action>(std::chrono::seconds{1}, Action::Kind::START, std::chrono::milliseconds{50}, mObjectId);
    auto action3 = std::make_shared<Action>(std::chrono::seconds{1}, Action::Kind::START, std::chrono::seconds{3}, mObjectId);

    getCoreP()->scheduleAction(std::move(action));
    getCoreP()->scheduleAction(std::move(action2));
    getCoreP()->scheduleAction(std::move(action3));
}

} // ns paresis