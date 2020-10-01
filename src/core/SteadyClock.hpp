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
    typedef int64_t rep;
    typedef std::nano period;
    typedef std::chrono::duration<rep, period> duration;
    typedef std::chrono::time_point<SteadyClock> time_point;

    SteadyClock(float simSpeed) : mSimSpeed(simSpeed), mSimTime(0) {
        mStartTime = std::chrono::steady_clock::now();
    }

    void restart() {
        mStartTime = std::chrono::steady_clock::now();
    }

    virtual std::chrono::nanoseconds getSimTimeNow() const;
    virtual void updateSimTime(std::chrono::nanoseconds);  // is called whenever the simulation executes an event
    virtual std::chrono::time_point<std::chrono::steady_clock> getRealTimeForCurrentSimTime(); // returns expected RealTime for given SimTime
    virtual std::chrono::nanoseconds getDurationUntil(std::chrono::nanoseconds);  // returns how many (real) nanoseconds have to be waited until the passed time is reached
    virtual std::chrono::time_point<std::chrono::steady_clock> getTimePointforSimTime(std::chrono::nanoseconds);

protected:
    float mSimSpeed;

    std::chrono::time_point<std::chrono::steady_clock> mStartTime;
    duration mSimTime; // in ns
};

} // ns paresis

#endif /* _STEADYCLOCK_HPP_LSDNFO */