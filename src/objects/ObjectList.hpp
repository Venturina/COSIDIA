#ifndef _OBJECT_LIST_NVOS
#define _OBJECT_LIST_NVOS

#include "objects/BaseObject.hpp"
#include <atomic>
#include <unordered_map>

namespace paresis
{

using ObjectContainer = std::unordered_map<int, std::shared_ptr<BaseObject>>;
using ObjectContainer_ptr = std::shared_ptr<ObjectContainer>;



class ObjectList
{
public:
    ObjectList();
    void addToObjectContainer(int objectId, std::shared_ptr<BaseObject>);
    void removeObjectById(int id);
    ObjectContainer_ptr getCurrentObjectList();
    std::atomic<long int>& getCurrentObjectId() { return mCurrentObjectId; }

private:
    ObjectContainer_ptr mWorkingCopy;
    ObjectContainer_ptr mCurrentCopy;

    std::atomic<long int> mCurrentObjectId{0};
};

}
#endif /* _OBJECT_LIST_NVOS */