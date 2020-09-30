#include "core/Core.hpp"
#include "objects/BaseObject.hpp"
#include "objects/ObjectList.hpp"
#include "utils/enforce.hpp"

namespace paresis
{

void ObjectContainer::insert(std::shared_ptr<BaseObject> obj)
{
    enforce(onCoreThread() == true, "ObjectContainer: insert on wrong core");
    enforce(obj->isInitialized(), "object not initialized")
    enforce(mActiveObjects.find(obj->getObjectId()) == mActiveObjects.end(), "object already in container");
    mActiveObjects[obj->getObjectId()] = obj;
}

void ObjectContainer::remove(int id)
{
    enforce(onCoreThread() == true, "ObjectContainer: remove on wrong core");
    enforce(mActiveObjects.find(id) != mActiveObjects.end(), "removed object not in container");
    mActiveObjects.erase(id);
}

void ObjectContainer::remove(std::shared_ptr<BaseObject> obj)
{
    enforce(onCoreThread() == true, "ObjectContainer: remove on wrong core");
    enforce(obj->isInitialized(), "object not initialized")
    enforce(mActiveObjects.find(obj->getObjectId()) != mActiveObjects.end(), "removed object not in container");
    remove(obj->getObjectId());
}

std::shared_ptr<BaseObject> ObjectContainer::getObject(int id)
{
    enforce(mActiveObjects.find(id) != mActiveObjects.end() || mRemovedObjects.find(id) != mRemovedObjects.end(), "ObjectContainer: requested objet not in container nor is it removed");
    if(mActiveObjects.find(id) == mActiveObjects.end()) {
        std::cout << "test" << std::endl;
        return nullptr;
    }
    return mActiveObjects[id];
}

void ObjectContainer::insertUnique(std::shared_ptr<BaseObject> obj)
{
    enforce(obj->isInitialized(), "object not initialized")
    enforce(mUniqueObjects.find(obj->getObjectName()) == mUniqueObjects.end(), "unique object already in container");
    enforce(mActiveObjects.find(obj->getObjectId()) == mActiveObjects.end(), "unique object already in container")
    mUniqueObjects[obj->getObjectName()] = obj->getObjectId();
    insert(std::move(obj));
}

std::shared_ptr<BaseObject> ObjectContainer::getUnique(std::string name)
{
    enforce(mUniqueObjects.find(name) != mUniqueObjects.end(), "requested unique object not in container");
    enforce(mActiveObjects.find(mUniqueObjects[name]) != mActiveObjects.end(), "requested unique missing in object container");
    return mActiveObjects[mUniqueObjects[name]];
}

void ObjectContainer::removeFromSimulation(int id) {
    mRemovedObjects[id] = mRemovedObjects[id]++;
}

ObjectList::ObjectList()
{
    mWorkingCopy = std::make_shared<ObjectContainer>();
    mCurrentCopy = std::make_shared<ObjectContainer>(*mWorkingCopy);
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

int ObjectList::getNextObjectId()
{
    mCurrentObjectId++;
    return mCurrentObjectId;
}

void ObjectList::removeFromSimulation(int id)
{
    mWorkingCopy->remove(id);
    mWorkingCopy->removeFromSimulation(id);
    mCurrentCopy = std::make_shared<ObjectContainer>(*mWorkingCopy);
}

}