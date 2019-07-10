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
    SteadyClock(float simSpeed) : mSimSpeed(simSpeed) {
        mStartTime = std::chrono::steady_clock::now();
    }

    void restart() {
        mStartTime = std::chrono::steady_clock::now();
    }

    uint64_t getSimTimeNow();
    void updateSimTime(uint64_t);  // is called whenever the simulation executes an event
    std::chrono::time_point<std::chrono::steady_clock> getRealTimeForSimTime(); // returns expected RealTime for given SimTime
    std::chrono::nanoseconds getDurationUntil(uint64_t);  // returns how many (real) nanoseconds have to be waited until the passed time is reached
    std::chrono::time_point<std::chrono::steady_clock>getTimePointforSimTime(uint64_t);

private:
    float mSimSpeed;

    std::chrono::time_point<std::chrono::steady_clock> mStartTime;
    uint64_t mSimTime = 0; // in ns
};

} // ns paresis

#endif /* _STEADYCLOCK_HPP_LSDNFO */