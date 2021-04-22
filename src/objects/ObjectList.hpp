#ifndef _OBJECT_LIST_NVOS
#define _OBJECT_LIST_NVOS

#include "objects/ObjectId.hpp"
#include <atomic>
#include <unordered_map>
#include <map>
#include <memory>
#include <set>
#include <typeindex>
#include <typeinfo>

namespace cosidia
{

class BaseObject;

//using ObjectContainer = std::unordered_map<int, std::shared_ptr<BaseObject>>;

class ObjectContainer
{
public:
    void insert(std::shared_ptr<BaseObject>);
    void remove(std::shared_ptr<BaseObject>);
    void remove(ObjectId id);
    std::shared_ptr<BaseObject> getObject(ObjectId id) const;

    void insertUnique(std::shared_ptr<BaseObject>);
    std::shared_ptr<BaseObject> getUnique(std::string) const;

    void removeFromSimulation(ObjectId);

    const std::map<ObjectId, std::shared_ptr<BaseObject>> getAll() const { return mActiveObjects; };

    void prettyPrint() const;

private:
    std::map<ObjectId, std::shared_ptr<BaseObject>> mActiveObjects;
    std::map<std::string, ObjectId> mUniqueObjects;
    std::set<ObjectId> mRemovedObjects;
};

using ObjectContainer_ptr = std::shared_ptr<ObjectContainer>;
using ConstObjectContainer_ptr = std::shared_ptr<const ObjectContainer>;

class ObjectList
{
public:
    ObjectList();
    void addToObjectContainer(ObjectId objectId, std::shared_ptr<BaseObject>);
    void addUnique(std::shared_ptr<BaseObject>);

    void removeObjectById(ObjectId id);

    void removeFromSimulation(ObjectId);
    std::shared_ptr<BaseObject>getUniqueObjectByName(std::string);
    std::shared_ptr<BaseObject>getObjectByIdFromCurrentContainer(ObjectId id);
    ConstObjectContainer_ptr getCurrentObjectContainer() const;
    ObjectId getNextObjectId();

    void prettyPrint() const { mCurrentCopy->prettyPrint(); }

private:
    ObjectContainer_ptr mWorkingCopy;
    ConstObjectContainer_ptr mCurrentCopy;

    int mCurrentObjectId = 1;
};

}
#endif /* _OBJECT_LIST_NVOS */