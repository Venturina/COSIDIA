#include "objects/ObjectList.hpp"

namespace paresis
{

void ObjectList::addToObjectContainer(std::shared_ptr<BaseObject> object)
{
    mWorkingCopy->push_back(object);  // modify current working copy, no changes for other threads
    mCurrentCopy = std::make_shared<ObjectContainerList>(*mWorkingCopy);
}

ObjectContainer ObjectList::getCurrentObjectList()
{
    return mCurrentCopy;
}

}