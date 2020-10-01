#include "core/DebugClock.hpp"

namespace paresis
{

std::chrono::time_point<std::chrono::steady_clock> DebugClock::getRealTimeForCurrentSimTime()
{
    return std::chrono::steady_clock::now();
}

std::chrono::nanoseconds DebugClock::getSimTimeNow() const
{
    return mSimTime;
}

std::chrono::nanoseconds DebugClock::getDurationUntil(std::chrono::nanoseconds until)
{
    auto currentTime = mSimTime;
    updateSimTime(until);
    return until - currentTime;
}

std::chrono::time_point<std::chrono::steady_clock> DebugClock::getTimePointforSimTime(std::chrono::nanoseconds until)
{
    return std::chrono::steady_clock::now() + getDurationUntil(until);
}


}