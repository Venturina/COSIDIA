#include "core/DebugClock.hpp"

namespace cosidia
{

SteadyClock::time_point DebugClock::getRealTimeForCurrentSimTime()
{
    return std::chrono::steady_clock::now();
}

SimClock::time_point DebugClock::getSimTimeNow() const
{
    return mSimTime;
}

SteadyClock::duration DebugClock::getDurationUntil(SimClock::time_point until)
{
    auto currentTime = mSimTime;
    updateSimTime(until);
    return until - currentTime;
}

SteadyClock::time_point DebugClock::getTimePointforSimTime(SimClock::time_point until)
{
    return std::chrono::steady_clock::now() + getDurationUntil(until);
}

}