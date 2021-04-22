#include "core/Core.hpp"
#include "core/InitAction.hpp"
#include "objects/BaseObject.hpp"
#include "objects/TemporaryObject.hpp"
#include "utils/enforce.hpp"

#include "loguru/loguru.hpp"

namespace cosidia
{

TemporaryObject::TemporaryObject(ObjectId id, std::shared_ptr<BaseObject> obj, bool initialized) :
    mBaseObject(obj), mTempId(id), mIsInitialized(initialized)
{
}

void TemporaryObject::setTempChild(const TemporaryObject& child)
{
    mTempChildren.emplace(child.getTempId());
}

void TemporaryObject::setTempParent(const TemporaryObject& parent)
{
    mTempParents.emplace(parent.getTempId());
}

void TemporaryObject::setTempMapping(ObjectId realId)
{
    mTempMapping.first = mTempId;
    mTempMapping.second = realId;
}

void TemporaryObject::setRealChildren(ObjectId realId)
{
    DLOG_F(ERROR, "Object: %s gets child: ", mBaseObject->getObjectName().c_str());
    mBaseObject->addChild(realId);
}

void TemporaryObject::setRealParent(ObjectId realId)
{
    DLOG_F(ERROR, "Object: %s gets parent: ", mBaseObject->getObjectName().c_str());
    mBaseObject->addParent(realId); // TODO parent as a list!
}
ObjectId TemporaryObject::getRealId()
{
    assert(mTempMapping.second.valid());
    return mTempMapping.second;
}

std::string TemporaryObject::getObjectName()
{
    return mBaseObject->getObjectName();
}

void TemporaryObject::addToCore()
{
    getCoreP()->addObject(mBaseObject);
}

void TemporaryObject::initialize(SimClock::time_point t)
{
    if(!isInitialized()) {
        auto id = getCoreP()->getNextObjectId();
        mBaseObject->setObjectId(id);
        setTempMapping(id);
        auto newAction = ActionFactory<InitAction>::create(std::chrono::milliseconds(50), t + std::chrono::milliseconds(1), id, id);
        newAction->scheduleStartHandler();
    } else {
        setTempMapping(mBaseObject->getObjectId());
    }
}


void TemporaryObjectList::resolveAndStart(SimClock::time_point start)
{
    DLOG_F(ERROR, "Try Resolve");
    for(auto& obj : mTempList) {
        enforce(obj, "TemporaryObjectList: Something wrong with the pointer");
        obj->initialize(start);
        mResolvedIds[obj->getTempId()] = obj;
    }

    DLOG_F(ERROR, "Added Ids");

    for(auto& obj : mTempList) {
        enforce(obj, "TemporaryObjectList: Something wrong with the pointer");
        for(auto children : obj->getTemporaryChildren()) {
            obj->setRealChildren(mResolvedIds[children]->getRealId());
            DLOG_F(ERROR, "           %s", mResolvedIds[children]->getObjectName().c_str());
        }
        for(auto parent : obj->getTemporaryParent()) {
            obj->setRealParent(mResolvedIds[parent]->getRealId());
            DLOG_F(ERROR, "           %s", mResolvedIds[parent]->getObjectName().c_str());
        }
        if(!obj->isInitialized()) {
            obj->addToCore();
        }
    }
}



}