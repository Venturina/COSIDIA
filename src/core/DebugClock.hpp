#ifndef _DEBUG_CLOCK_HPP_SDNFO
#define _DEBUG_CLOCK_HPP_SDNFO

#include "core/SteadyClock.hpp"

namespace paresis
{

class DebugClock : public SteadyClock
{
public:
    DebugClock(float simSpeed) : SteadyClock(simSpeed) {};

    virtual std::chrono::time_point<std::chrono::steady_clock> getRealTimeForCurrentSimTime() override;
    virtual std::chrono::nanoseconds getSimTimeNow() const override;
    virtual std::chrono::nanoseconds getDurationUntil(std::chrono::nanoseconds) override;
    virtual std::chrono::time_point<std::chrono::steady_clock> getTimePointforSimTime(std::chrono::nanoseconds) override;


private:

};

} // namespace paresis

#endif /* _DEBUG_CLOCK_HPP_SDNFO */
