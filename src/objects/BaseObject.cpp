#include "core/Core.hpp"
#include "objects/BaseObject.hpp"
#include "utils/enforce.hpp"

#include <loguru/loguru.hpp>

namespace paresis
{

BaseObject::BaseObject() : mActionManager(new ObjectActionManager())
{
}

BaseObject::~BaseObject()
{
    DLOG_F(ERROR, "Called Destructor of %d, %s", mObjectId, mObjectName.c_str());
}

int BaseObject::execute(std::shared_ptr<Action> action)
{
    enforce(isInitialized(), "Tried to execute a not initialized object");

    switch (action->getKind()) {
        case Action::Kind::START:
            if(!mActionManager->startOrDelay(action)) {
                //getCoreP()->scheduleAction(makeEndAction(action));
                startExecution(std::move(action));
                return mObjectId;
            }
            return 0;
        case Action::Kind::END:
            {
                enforce(action->getBeginId() != 0, "BaseObject: EndAction does not correspond to a begin action");
                endExecution(action);
                auto now = getCoreP()->getClock().getSimTimeNow();
                timingBuffer[currId++] = (action->getStartTime().count() - now.count()) / 1000;
                //DLOG_F(ERROR, "time: expected: %d now: %d value: %d, type %s", action->getStartTime().count()/1000, now.count()/1000, (action->getStartTime().count() - now.count()) / 1000, mObjectName.c_str());
                // if(!((action->getStartTime() < std::chrono::seconds(3)) ||
                //     (action->getStartTime() - now) > std::chrono::milliseconds(-1))) {
                //     throw std::runtime_error("Time Violation");
                // }
                if(mActionManager->endAndCheckAvailable()) {
                    auto update = mActionManager->fetchNextAction();
                    update.setStartTime(action->getStartTime() + action->getDuration());
                    auto next = mActionManager->activateNextAvailableAction();
                    getCoreP()->scheduleAction(makeEndAction(next));
                    startExecution(std::move(next));
                }
                return 0;
            }
        case Action::Kind::INIT:
            initObject(action);
            return 0;
        default:
            return 0;
    }
}

bool BaseObject::isInitialized()
{
    if(mObjectName.compare("") == 0 || mObjectId == -1 || mParentList.empty()) {
        return false;
    } else {
        return true;
    }
}

std::shared_ptr<Action> BaseObject::createSelfAction(SteadyClock::duration duration, SteadyClock::duration start)
{
    return std::move(std::make_shared<Action>(duration, Action::Kind::START, start, mObjectId, mObjectId));
}

std::weak_ptr<BaseObject> getSiblingByName(BaseObject* obj, std::string name, ConstObjectContainer_ptr objects)
{
    for(auto parentId : obj->getParents()) {
        const auto parent = objects->getObject(parentId);
        for(const auto siblingId : parent->getChildren()) {
            const auto sibling = objects->getObject(siblingId);
            if(sibling->getObjectName() == name) {
                return std::weak_ptr(sibling);
            }
        }
    }
    enforce(false, "getSiblingByName failed")
    return std::weak_ptr<BaseObject>();
}

std::weak_ptr<BaseObject> getParentByName(BaseObject* obj, std::string name, ConstObjectContainer_ptr objects)
{
    for(const auto parentId : obj->getParents()) {
        auto parent = objects->getObject(parentId);
        if(parent->getObjectName() == name) {
            return std::weak_ptr(parent);
        }
    }
    enforce(false, "getParentByName failed")
    return std::weak_ptr<BaseObject>();
}

std::weak_ptr<BaseObject> getChildByName(BaseObject* obj, std::string name, ConstObjectContainer_ptr objects)
{
    for(const auto childId : obj->getChildren()) {
        auto child = objects->getObject(childId);
        if(child->getObjectName() == name) {
            return std::weak_ptr(child);
        }
    }
    enforce(false, "getChildByName failed")
    return std::weak_ptr<BaseObject>();
}

int getRelatedObjectByName(BaseObject* obj, std::string name, ConstObjectContainer_ptr objects)
{
   return 0;
}


}