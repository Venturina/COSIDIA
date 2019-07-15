#ifndef _STEADYCLOCK_HPP_LSDNFO
#define _STEADYCLOCK_HPP_LSDNFO

#include <boost/chrono/chrono.hpp>
#include <chrono>
#include <iostream>
#include <iomanip>

namespace paresis
{

class SteadyClock
{
public:
    SteadyClock(float simSpeed) : mSimSpeed(simSpeed), mSimTime(0) {
        mStartTime = std::chrono::steady_clock::now();
    }

    void restart() {
        mStartTime = std::chrono::steady_clock::now();
    }

    std::chrono::nanoseconds getSimTimeNow();
    void updateSimTime(std::chrono::nanoseconds);  // is called whenever the simulation executes an event
    std::chrono::time_point<std::chrono::steady_clock> getRealTimeForSimTime(); // returns expected RealTime for given SimTime
    std::chrono::nanoseconds getDurationUntil(std::chrono::nanoseconds);  // returns how many (real) nanoseconds have to be waited until the passed time is reached
    std::chrono::time_point<std::chrono::steady_clock> getTimePointforSimTime(std::chrono::nanoseconds);

private:
    float mSimSpeed;

    std::chrono::time_point<std::chrono::steady_clock> mStartTime;
    std::chrono::nanoseconds mSimTime; // in ns
};

} // ns paresis

#endif /* _STEADYCLOCK_HPP_LSDNFO */