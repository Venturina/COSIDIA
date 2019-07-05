#ifndef _CLOCK_HPP_LSDNFO
#define _CLOCK_HPP_LSDNFO

#include <chrono>

namespace paresis
{

class Clock
{
public:
    Clock(float simSpeed) : mSimSpeed(simSpeed) {
        mStartTime = std::chrono::high_resolution_clock::now();
    }

    uint64_t getSimTimeNow();
    void updateSimTime(uint64_t);  // is called whenever the simulation executes an event
    std::chrono::time_point<std::chrono::high_resolution_clock> getRealTimeForSimTime(); // returns expected RealTime for given SimTime
    std::chrono::nanoseconds getDurationUntil(uint64_t);  // returns how many (real) nanoseconds have to be waited until the passed time is reached
    std::chrono::time_point<std::chrono::high_resolution_clock>getTimePointforSimTime(uint64_t);

private:
    float mSimSpeed;

    std::chrono::time_point<std::chrono::high_resolution_clock> mStartTime;
    uint64_t mSimTime = 0; // in ns
};

} // ns paresis

#endif /* _CLOCK_HPP_LSDNFO */