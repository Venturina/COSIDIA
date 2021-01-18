#include "core/SteadyClock.hpp"
#include "utils/enforce.hpp"
#include <cmath>

namespace cosidia
{

SteadyClock::SteadyClock(float simSpeed) : mSimSpeed(simSpeed), mSimTime(SimClock::duration::zero())
{
    mStartTime = std::chrono::steady_clock::now();
}

void SteadyClock::restart()
{
    mStartTime = std::chrono::steady_clock::now();
}

void SteadyClock::updateSimTime(SimClock::time_point newTime)
{
    enforce(newTime >= mSimTime, "clock updates can only advance forward in time");
    mSimTime = newTime;
}

SteadyClock::time_point SteadyClock::getRealTimeForCurrentSimTime()
{
    updateSimTime(getSimTimeNow());
    return mStartTime + std::chrono::duration_cast<SteadyClock::duration>(mSimTime.time_since_epoch() / mSimSpeed);
}

SimClock::time_point SteadyClock::getSimTimeNow() const
{
    duration elapsedRealTime = std::chrono::steady_clock::now() - mStartTime;
    auto elapsedSimTime = std::chrono::duration_cast<SimClock::duration>(elapsedRealTime * mSimSpeed);
    return SimClock::time_point { elapsedSimTime };
}

SteadyClock::duration SteadyClock::getDurationUntil(SimClock::time_point until)
{
    updateSimTime(getSimTimeNow());
    //auto u = std::chrono::nanoseconds(static_cast<uint64_t>((until - mSimTime) * mSimSpeed));
    std::chrono::nanoseconds s = mSimTime - until;
    //std::cout << (int64_t)mSimTime << std::endl;
    //std::cout << until << std::endl;
    //std::cout << s << std::endl;
    return std::chrono::duration_cast<std::chrono::nanoseconds>((until - mSimTime) * mSimSpeed);
}

SteadyClock::time_point SteadyClock::getTimePointforSimTime(SimClock::time_point until)
{
    auto u = getDurationUntil(until);
    //std::cout << u.count() << std::endl;
    auto til = mStartTime + u - std::chrono::steady_clock::now();
    //std::cout << til.count() << std::endl;
    return mStartTime + getDurationUntil(until);
}

} // ns cosidia