#ifndef _OBJECT_LIST_NVOS
#define _OBJECT_LIST_NVOS

#include "objects/BaseObject.hpp"
#include <atomic>
#include <list>

struct ObjectContainer
{
    std::atomic_int counter;
    std::list<std::shared_ptr<BaseObject>> objectList;
};


class ObjectList
{
    void addToObjectContainer(BaseObject&);
    void removeObjectById(int id);

private:
    ObjectContainer* mWorkingCopy;
    ObjectContainer* mCurrentCopy;
    ObjectContainer* mDeprecatedCopy;
};


#endif /* _OBJECT_LIST_NVOS */