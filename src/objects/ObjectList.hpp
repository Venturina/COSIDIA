#ifndef _OBJECT_LIST_NVOS
#define _OBJECT_LIST_NVOS

#include "objects/BaseObject.hpp"
#include <atomic>
#include <unordered_map>

namespace paresis
{

using ObjectContainer = std::unordered_map<int, std::shared_ptr<BaseObject>>;
using ObjectContainer_ptr = std::shared_ptr<ObjectContainer>;
using AtomicObjectId = std::atomic<long int>;



class ObjectList
{
public:
    ObjectList();
    void addToObjectContainer(int objectId, std::shared_ptr<BaseObject>);
    void removeObjectById(int id);
    ObjectContainer_ptr getCurrentObjectList();
    AtomicObjectId& getCurrentObjectId() { return mCurrentObjectId; }

private:
    ObjectContainer_ptr mWorkingCopy;
    ObjectContainer_ptr mCurrentCopy;

    AtomicObjectId mCurrentObjectId{1};
};

}
#endif /* _OBJECT_LIST_NVOS */