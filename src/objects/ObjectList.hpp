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

    const std::map<int, std::shared_ptr<BaseObject>> getAll() { return data; };

private:
    std::map<int, std::shared_ptr<BaseObject>> data;
    std::map<std::string, int32_t> mUniqueObjects;
};

using ObjectContainer_ptr = std::shared_ptr<ObjectContainer>;
using AtomicObjectId = std::atomic<long int>;

class ObjectList
{
public:
    ObjectList();
    void addToObjectContainer(int objectId, std::shared_ptr<BaseObject>);
    void addUnique(std::shared_ptr<BaseObject>);

    void removeObjectById(int id);
    std::shared_ptr<BaseObject>getUniqueObjectByName(std::string);
    std::shared_ptr<BaseObject>getObjectByIdFromCurrentContainer(int id);
    ObjectContainer_ptr getCurrentObjectContainer();
    AtomicObjectId& getCurrentObjectId() { return mCurrentObjectId; }

private:
    ObjectContainer_ptr mWorkingCopy;
    ObjectContainer_ptr mCurrentCopy;

    AtomicObjectId mCurrentObjectId{1};
};

}
#endif /* _OBJECT_LIST_NVOS */