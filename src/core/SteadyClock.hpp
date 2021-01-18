#ifndef _STEADYCLOCK_HPP_LSDNFO
#define _STEADYCLOCK_HPP_LSDNFO

#include <boost/chrono/chrono.hpp>
#include <chrono>
#include <iostream>
#include <iomanip>

namespace cosidia
{

class SimClock
{
public:
    using rep = int64_t;
    using period = std::nano;
    using duration = std::chrono::duration<rep, period>;
    using time_point = std::chrono::time_point<SimClock>;

    template<typename Rep, typename Period>
    constexpr static time_point at(std::chrono::duration<Rep, Period> d)
    {
        return time_point { d };
    }

    constexpr static time_point atSecond(SimClock::rep sec)
    {
        return at(std::chrono::seconds(sec));
    }

    constexpr static time_point atMillisecond(SimClock::rep msec)
    {
        return at(std::chrono::milliseconds(msec));
    }

    constexpr static time_point atMicrosecond(SimClock::rep usec)
    {
        return at(std::chrono::microseconds(usec));
    }

    constexpr static time_point atNanosecond(SimClock::rep nsec)
    {
        return at(std::chrono::nanoseconds(nsec));
    }

    constexpr static rep getMilliseconds(time_point t)
    {
        using factor = std::ratio_divide<period, std::milli>;
        return t.time_since_epoch().count() * factor::num / factor::den;
    }
};

class SteadyClock
{
public:
    using duration = std::chrono::steady_clock::duration;
    using time_point = std::chrono::steady_clock::time_point;

    SteadyClock(float simSpeed);

    void restart();

    virtual SimClock::time_point getSimTimeNow() const;
    virtual void updateSimTime(SimClock::time_point);  // is called whenever the simulation executes an event
    virtual time_point getRealTimeForCurrentSimTime(); // returns expected RealTime for given SimTime
    virtual duration getDurationUntil(SimClock::time_point);  // returns how many (real) nanoseconds have to be waited until the passed time is reached
    virtual time_point getTimePointforSimTime(SimClock::time_point);

protected:
    float mSimSpeed;

    time_point mStartTime;
    SimClock::time_point mSimTime; // in ns
};

} // ns cosidia

#endif /* _STEADYCLOCK_HPP_LSDNFO */