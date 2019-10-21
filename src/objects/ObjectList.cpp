#include "objects/BaseObject.hpp"
#include "objects/ObjectList.hpp"

namespace paresis
{

void ObjectContainer::insert(std::shared_ptr<BaseObject> obj)
{
    assert(data.find(obj->getObjectId()) == data.end());
    data[obj->getObjectId()] = obj;
}

void ObjectContainer::remove(int id)
{
    assert(data.find(id) != data.end());
    data.erase(id);
}

std::shared_ptr<BaseObject> ObjectContainer::getObject(int id)
{
    assert(data.find(id) != data.end());
    return data[id];
}

void ObjectContainer::insertUnique(std::shared_ptr<BaseObject> obj)
{
    assert(mUniqueObjects.find(obj->getObjectName()) == mUniqueObjects.end());
    mUniqueObjects[obj->getObjectName()] = obj->getObjectId();
    insert(std::move(obj));
}

std::shared_ptr<BaseObject> ObjectContainer::getUnique(std::string name)
{
    assert(mUniqueObjects.find(name) != mUniqueObjects.end());
    assert(data.find(mUniqueObjects[name]) == data.end());
    return data[mUniqueObjects[name]];
}

ObjectList::ObjectList()
{
    mWorkingCopy = std::make_shared<ObjectContainer>();
}

void ObjectList::addToObjectContainer(int objectId, std::shared_ptr<BaseObject> object)
{
    //(*mWorkingCopy)[objectId] = object;  // modify current working copy, no changes for other threads
    mWorkingCopy->insert(object);
    mCurrentCopy = std::make_shared<ObjectContainer>(*mWorkingCopy);
}

void ObjectList::addUnique(std::shared_ptr<BaseObject> obj)
{
    mWorkingCopy->insertUnique(obj);
    mCurrentCopy = std::make_shared<ObjectContainer>(*mWorkingCopy);
}

void ObjectList::removeObjectById(int id)
{
    mWorkingCopy->remove(id);
    mCurrentCopy = std::make_shared<ObjectContainer>(*mWorkingCopy);
}

std::shared_ptr<BaseObject> ObjectList::getUniqueObjectByName(std::string name)
{
    return mCurrentCopy->getUnique(name);
}

std::shared_ptr<BaseObject> ObjectList::getObjectByIdFromCurrentContainer(int id)
{
    //assert(mCurrentCopy->getObject(id));
    return mCurrentCopy->getObject(id);
}

ObjectContainer_ptr ObjectList::getCurrentObjectContainer()
{
    return mCurrentCopy;
}

}