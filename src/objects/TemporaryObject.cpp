#include "core/Core.hpp"
#include "objects/BaseObject.hpp"
#include "objects/TemporaryObject.hpp"


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
    mBaseObject->addChild(realId);
}

void TemporaryObject::setRealParent(int realId)
{
    mBaseObject->setParent(realId); // TODO parent as a list!
}
int TemporaryObject::getRealId()
{
    assert(mTempMapping.second != -1);
    return mTempMapping.second;
}

void TemporaryObject::initialize(SteadyClock::duration d)
{
    auto id = getCoreP()->getNextObjectId();
    mBaseObject->setObjectId(id);
    setTempMapping(id);
    mIsInitialized = true;
    auto newAction = std::make_shared<Action>(std::chrono::milliseconds(50), Action::Kind::START, d + std::chrono::milliseconds(50), id);
}

void TemporaryObjectList::resolveAndStart(SteadyClock::duration d)
{
    for(auto obj : mTempList) {
        if(!obj->isInitialized()) {
            obj->initialize(d);
            mResolvedIds[obj->getRealId()] = obj;
        }
    }

    for(auto obj : mTempList) {
        assert(obj->isInitialized());
        for(auto children : obj->getTemporaryChildren()) {
            obj->setRealChildren(mResolvedIds[children]->getRealId());
        }
        for(auto parent : obj->getTemporaryParent()) {
            obj->setRealParent(mResolvedIds[parent]->getRealId());
        }
    }
}



}