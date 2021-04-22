#include "networking/ActiveActionTracker.hpp"
#include "utils/enforce.hpp"

namespace cosidia
{

void ActiveActionTracker::setNext(const Action* action)
{
    mNextStart = action;
    if(!mNextEnd) {
        mNextEnd = action;
    }
}

bool ActiveActionTracker::isEnd(const Action* action)
{
    if(mNextEnd == action || !mNextEnd) {
        enforce(mNextEnd != mNextStart || !mNextEnd && !mNextStart, "ActiveActionTracker: start and end must be different");
        mNextEnd = mNextStart;
        return true;
    } else {
        return false;
    }
}

bool ActiveActionTracker::isStart(const Action* action) const
{
    if(mNextStart == action || !mNextStart) {
        return true;
    } else {
        return false;
    }
}

SimClock::time_point ActiveActionTracker::nextStart() const
{
    if(mNextStart) {
        return mNextStart->getStartTime();
    }
    return SimClock::zero();
}

const Action* ActiveActionTracker::getEnd() const
{
    return mNextEnd;
}

} // namespace cosidia
