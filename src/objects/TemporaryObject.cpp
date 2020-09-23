#include "core/Core.hpp"
#include "objects/BaseObject.hpp"
#include "objects/TemporaryObject.hpp"

#include "loguru/loguru.hpp"

namespace paresis
{

TemporaryObject::TemporaryObject(int id, std::shared_ptr<BaseObject> obj, bool initialized) :
    mBaseObject(obj), mTempId(id), mIsInitialized(initialized)
{
}

void TemporaryObject::setTempChild(int child)
{
    mTempChildren.emplace(child);
}

void TemporaryObject::setTempParent(int parent)
{
    mTempParents.emplace(parent);
}

void TemporaryObject::setTempMapping(int realId)
{
    mTempMapping.first = mTempId;
    mTempMapping.second = realId;
}

void TemporaryObject::setRealChildren(int realId)
{
    DLOG_F(ERROR, "Object: %s gets child: ", mBaseObject->getObjectName().c_str());
    mBaseObject->addChild(realId);
}

void TemporaryObject::setRealParent(int realId)
{
    DLOG_F(ERROR, "Object: %s gets parent: ", mBaseObject->getObjectName().c_str());
    mBaseObject->addParent(realId); // TODO parent as a list!
}
int TemporaryObject::getRealId()
{
    assert(mTempMapping.second != -1);
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

void TemporaryObject::initialize(SteadyClock::duration d)
{
    if(!isInitialized()) {
        auto id = getCoreP()->getNextObjectId();
        mBaseObject->setObjectId(id);
        setTempMapping(id);
        auto newAction = std::make_shared<Action>(std::chrono::milliseconds(50), Action::Kind::INIT, d + std::chrono::milliseconds(1), id);

        getCoreP()->scheduleAction(newAction);
    } else {
        setTempMapping(mBaseObject->getObjectId());
    }
}


void TemporaryObjectList::resolveAndStart(SteadyClock::duration d)
{
    DLOG_F(ERROR, "Try Resolve");
    for(auto& obj : mTempList) {
        obj->initialize(d);
        mResolvedIds[obj->getTempId()] = obj;
    }

    DLOG_F(ERROR, "Added Ids");

    for(auto& obj : mTempList) {
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