#include "core/Core.hpp"
#include "debug/DebugObject.hpp"
#include "loguru/loguru.hpp"

namespace cosidia
{
DebugObject::DebugObject() : BaseObject()
{
}

void DebugObject::startExecution(std::shared_ptr<Action>)
{
    DLOG_F(INFO, "DebugParent Event Start");
}

void DebugObject::endExecution(std::shared_ptr<Action>)
{
    DLOG_F(INFO, "DebugParent Event End");
}

void DebugObject::setName(std::string name)
{
    mObjectName = name;
}

std::shared_ptr<Action> DebugObject::testCreateSelfAction(SimClock::duration duration, SimClock::time_point start)
{
    return createSelfAction(duration, start);
}

DebugObjectParent::DebugObjectParent() : DebugObject()
{
}

void DebugObjectParent::initObject()
{
    mObjectName = "DebugParentObject";
    mObjectId = ObjectId { 0 };
    mParentList.insert(ObjectId { 0 });
}

void DebugObjectParent::initObject(std::shared_ptr<Action>)
{
    mObjectName = "DebugParentObject";
    mObjectId = ObjectId { 0 };
    mParentList.insert(ObjectId { 0 });
}

DebugObjectChild::DebugObjectChild() : DebugObject()
{
}

void DebugObjectChild::initObject()
{
    LOG_F(INFO, "Child Initialized");
    mObjectName = "DebugChildObject";
    mObjectId = ObjectId { 1 };
    mParentList.insert(ObjectId { 0 });
}

void DebugObjectChild::initObject(std::shared_ptr<Action>)
{
    LOG_F(INFO, "Child Initialized");
    mObjectName = "DebugChildObject";
    mObjectId = ObjectId { 1 };
    mParentList.insert(ObjectId { 0 });
}


} // ns cosidia