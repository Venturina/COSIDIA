#include "core/Clock.hpp"
#include <cassert>
#include <cmath>

namespace paresis
{
namespace core
{

void Clock::updateSimTime(uint64_t newTime)
{
    assert(newTime >= mSimTime);
    mSimTime = newTime;
}

std::chrono::time_point<std::chrono::high_resolution_clock> Clock::getRealTimeForSimTime()
{
    return mStartTime + std::chrono::nanoseconds(static_cast<uint64_t>(mSimTime * mSimSpeed));
}

uint64_t Clock::getSimTimeNow()
{
    auto elapsedRealTime = std::chrono::high_resolution_clock::now() - mStartTime;
    auto elapsedSimTime = elapsedRealTime * mSimSpeed;
    return elapsedSimTime.count();
}

std::chrono::nanoseconds Clock::getDurationUntil(uint64_t until)
{
    updateSimTime(getSimTimeNow());
    return std::chrono::nanoseconds(static_cast<uint64_t>((until - mSimTime) * mSimSpeed));
}

std::chrono::time_point<std::chrono::high_resolution_clock> Clock::getTimePointforSimTime(uint64_t until)
{
    return mStartTime + getDurationUntil(until);
}

} // ns core
} // ns paresis