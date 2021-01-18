#include "core/Core.hpp"
#include "objects/BaseObject.hpp"
#include "utils/enforce.hpp"

#include <loguru/loguru.hpp>

namespace cosidia
{

BaseObject::BaseObject() :
    mObjectId(ObjectId::stub()),
    mActionManager(new ObjectActionManager())
{
}

BaseObject::~BaseObject()
{
    DLOG_F(ERROR, "Called Destructor of %d, %s", mObjectId, mObjectName.c_str());
}

ObjectId BaseObject::execute(std::shared_ptr<Action> action)
{
    enforce(isInitialized(), "Tried to execute a not initialized object");
    switch (action->getKind()) {
        case Action::Kind::START:
            if(!mActionManager->startOrDelay(action)) {
                //getCoreP()->scheduleAction(makeEndAction(action));
                #ifdef COSIDIA_SAFE
                mCurrentAction = action->getActionId();
                #endif
                startExecution(std::move(action));
                return mObjectId;
            }
            return ObjectId::stub();
        case Action::Kind::END:
            {
                enforce(action->getBeginId() != 0, "BaseObject: EndAction does not correspond to a begin action");
                enforce(mCurrentAction == action->getBeginId(), "BaseObject: EndAction does not correspond to StartAction");
                endExecution(action);
                auto now = getCoreP()->getClock().getSimTimeNow();
                timingBuffer[currId++] = (action->getStartTime() - now).count() / 1000;
                //DLOG_F(ERROR, "time: expected: %d now: %d value: %d, type %s", action->getStartTime().count()/1000, now.count()/1000, (action->getStartTime().count() - now.count()) / 1000, mObjectName.c_str());
                // if(!((action->getStartTime() < std::chrono::seconds(3)) ||
                //     (action->getStartTime() - now) > std::chrono::milliseconds(-1))) {
                //     throw std::runtime_error("Time Violation");
                // }
                if(mActionManager->endAndCheckAvailable()) {
                    auto update = mActionManager->fetchNextAction();
                    update.setStartTime(action->getStartTime() + action->getDuration());
                    auto next = mActionManager->activateNextAvailableAction();
                    getCoreP()->scheduleAction(makeEndAction(next, std::list<ObjectId> {mObjectId}) );
                    #ifdef COSIDIA_SAFE
                    mCurrentAction = update.getActionId();
                    #endif
                    startExecution(std::move(next));
                }
                return ObjectId::stub();
            }
        case Action::Kind::INIT:
            initObject(action);
            return ObjectId::stub();
        default:
            return ObjectId::stub();
    }
}

bool BaseObject::isInitialized()
{
    if(mObjectName.compare("") == 0 || !mObjectId.valid() || mParentList.empty()) {
        return false;
    } else {
        return true;
    }
}

std::shared_ptr<Action> BaseObject::createSelfAction(SimClock::duration duration, SimClock::time_point start)
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
                return std::weak_ptr<BaseObject>(sibling);
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
            return std::weak_ptr<BaseObject>(parent);
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