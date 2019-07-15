#include "core/SteadyClock.hpp"
#include <cassert>
#include <cmath>

namespace paresis
{

void SteadyClock::updateSimTime(std::chrono::nanoseconds newTime)
{
    assert(newTime >= mSimTime);
    mSimTime = newTime;
}

std::chrono::time_point<std::chrono::steady_clock> SteadyClock::getRealTimeForSimTime()
{
    return mStartTime + std::chrono::duration_cast<std::chrono::nanoseconds>(mSimTime * mSimSpeed);
}

std::chrono::nanoseconds SteadyClock::getSimTimeNow()
{
    std::chrono::nanoseconds elapsedRealTime = std::chrono::steady_clock::now() - mStartTime;
    std::chrono::nanoseconds elapsedSimTime = std::chrono::duration_cast<std::chrono::nanoseconds>(elapsedRealTime * mSimSpeed);
    return elapsedSimTime;
}

std::chrono::nanoseconds SteadyClock::getDurationUntil(std::chrono::nanoseconds until)
{
    updateSimTime(getSimTimeNow());
    //auto u = std::chrono::nanoseconds(static_cast<uint64_t>((until - mSimTime) * mSimSpeed));
    std::chrono::nanoseconds s = mSimTime - until;
    //std::cout << (int64_t)mSimTime << std::endl;
    //std::cout << until << std::endl;
    //std::cout << s << std::endl;
    return std::chrono::duration_cast<std::chrono::nanoseconds>((until - mSimTime) * mSimSpeed);
}

std::chrono::time_point<std::chrono::steady_clock> SteadyClock::getTimePointforSimTime(std::chrono::nanoseconds until)
{
    auto u = getDurationUntil(until);
    //std::cout << u.count() << std::endl;
    auto til = mStartTime + u - std::chrono::steady_clock::now();
    //std::cout << til.count() << std::endl;
    return mStartTime + getDurationUntil(until);
}

} // ns paresis