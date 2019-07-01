#include "core/ObjectList.hpp"

void ObjectList::addToObjectContainer(BaseObject& object)
{
    mWorkingCopy->objectList.push_back(std::make_shared<BaseObject>(object));  // modify current working copy, no changes for other threads
    int expected = 0;
    int desired = 0;
    if(mDeprecatedCopy->counter.compare_exchange_weak(expected, desired,std::memory_order_release, std::memory_order_relaxed)) {
        throw std::runtime_error("mDeprecatedCopy is still in use!");
    } else {
        delete mDeprecatedCopy;
        mDeprecatedCopy = mCurrentCopy; // current copy turns to deprecated
        mCurrentCopy = mWorkingCopy; // current turns to working
    }
}