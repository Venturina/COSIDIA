#ifndef _OBJECT_LIST_NVOS
#define _OBJECT_LIST_NVOS

#include "objects/BaseObject.hpp"
#include <atomic>
#include <vector>

namespace paresis
{

using ObjectContainerVector = std::vector<std::shared_ptr<BaseObject>>;
using ObjectContainer = std::shared_ptr<ObjectContainerVector>;



class ObjectList
{
public:
    void addToObjectContainer(std::shared_ptr<BaseObject>);
    void removeObjectById(int id);
    ObjectContainer getCurrentObjectList();
    std::atomic<long int>& getCurrentObjectId() { return mCurrentObjectId; }

private:
    ObjectContainer mWorkingCopy;
    ObjectContainer mCurrentCopy;

    std::atomic<long int> mCurrentObjectId{0};
};

}
#endif /* _OBJECT_LIST_NVOS */