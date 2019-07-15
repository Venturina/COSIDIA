#ifndef _OBJECT_LIST_NVOS
#define _OBJECT_LIST_NVOS

#include "objects/BaseObject.hpp"
#include <atomic>
#include <list>

namespace paresis
{

using ObjectContainer = std::shared_ptr<std::list<std::shared_ptr<BaseObject>>>;
using ObjectContainerList = std::list<std::shared_ptr<BaseObject>>;


class ObjectList
{
    void addToObjectContainer(std::shared_ptr<BaseObject>);
    void removeObjectById(int id);
    ObjectContainer getCurrentObjectList();

private:
    ObjectContainer mWorkingCopy;
    ObjectContainer mCurrentCopy;
};

}
#endif /* _OBJECT_LIST_NVOS */