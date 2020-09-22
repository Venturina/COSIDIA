#ifndef _OBJECT_LIST_NVOS
#define _OBJECT_LIST_NVOS

#include <atomic>
#include <unordered_map>
#include <map>
#include <memory>
#include <typeindex>
#include <typeinfo>

namespace paresis
{

class BaseObject;

//using ObjectContainer = std::unordered_map<int, std::shared_ptr<BaseObject>>;

class ObjectContainer
{
public:
    void insert(std::shared_ptr<BaseObject>);
    void remove(std::shared_ptr<BaseObject>);
    void remove(int id);
    std::shared_ptr<BaseObject> getObject(int id);

    void insertUnique(std::shared_ptr<BaseObject>);
    std::shared_ptr<BaseObject> getUnique(std::string);

    void removeFromSimulation(int);

    const std::map<int, std::shared_ptr<BaseObject>> getAll() { return mActiveObjects; };

private:
    std::map<int, std::shared_ptr<BaseObject>> mActiveObjects;
    std::map<std::string, int32_t> mUniqueObjects;
    std::map<int, int> mRemovedObjects;
};

using ObjectContainer_ptr = std::shared_ptr<ObjectContainer>;
using AtomicObjectId = int;

class ObjectList
{
public:
    ObjectList();
    void addToObjectContainer(int objectId, std::shared_ptr<BaseObject>);
    void addUnique(std::shared_ptr<BaseObject>);

    void removeObjectById(int id);

    void removeFromSimulation(int);
    std::shared_ptr<BaseObject>getUniqueObjectByName(std::string);
    std::shared_ptr<BaseObject>getObjectByIdFromCurrentContainer(int id);
    ObjectContainer_ptr getCurrentObjectContainer();
    int getNextObjectId();
    //int getCurrentObjectId() { return mCurrentObjectId; }

private:
    ObjectContainer_ptr mWorkingCopy;
    ObjectContainer_ptr mCurrentCopy;

    AtomicObjectId mCurrentObjectId{1};
};

}
#endif /* _OBJECT_LIST_NVOS */