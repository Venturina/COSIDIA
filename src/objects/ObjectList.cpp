#include "objects/ObjectList.hpp"

namespace paresis
{
namespace object
{

void ObjectList::addToObjectContainer(BaseObject& object)
{
    mWorkingCopy->push_back(std::make_shared<BaseObject>(object));  // modify current working copy, no changes for other threads
    mCurrentCopy = std::make_shared<ObjectContainerList>(*mWorkingCopy);
}

ObjectContainer ObjectList::getCurrentObjectList()
{
    return mCurrentCopy;
}

}
}