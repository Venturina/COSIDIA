#include "objects/ObjectList.hpp"

namespace paresis
{

ObjectList::ObjectList()
{
    mWorkingCopy = std::make_shared<ObjectContainer>();
}

void ObjectList::addToObjectContainer(int objectId, std::shared_ptr<BaseObject> object)
{
    (*mWorkingCopy)[objectId] = object;  // modify current working copy, no changes for other threads
    mCurrentCopy = std::make_shared<ObjectContainer>(*mWorkingCopy);
}

ObjectContainer_ptr ObjectList::getCurrentObjectList()
{
    return mCurrentCopy;
}

}