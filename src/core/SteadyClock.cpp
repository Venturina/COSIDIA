#include "core/SteadyClock.hpp"
#include <cassert>
#include <cmath>

namespace paresis
{

void SteadyClock::updateSimTime(uint64_t newTime)
{
    assert(newTime >= mSimTime);
    mSimTime = newTime;
}

std::chrono::time_point<std::chrono::steady_clock> SteadyClock::getRealTimeForSimTime()
{
    return mStartTime + std::chrono::nanoseconds(static_cast<uint64_t>(mSimTime * mSimSpeed));
}

uint64_t SteadyClock::getSimTimeNow()
{
    auto elapsedRealTime = std::chrono::steady_clock::now() - mStartTime;
    auto elapsedSimTime = elapsedRealTime * mSimSpeed;
    return elapsedSimTime.count();
}

std::chrono::nanoseconds SteadyClock::getDurationUntil(uint64_t until)
{
    updateSimTime(getSimTimeNow());
    auto u = std::chrono::nanoseconds(static_cast<uint64_t>((until - mSimTime) * mSimSpeed));
    std::cout << u.count() << std::endl;
    return std::chrono::nanoseconds(static_cast<uint64_t>((until - mSimTime) * mSimSpeed));
}

std::chrono::time_point<std::chrono::steady_clock> SteadyClock::getTimePointforSimTime(uint64_t until)
{
    auto u = getDurationUntil(until);
    //std::cout << u.count() << std::endl;
    auto til = mStartTime + u - std::chrono::steady_clock::now();
    //std::cout << til.count() << std::endl;
    return mStartTime + getDurationUntil(until);
}

} // ns paresis