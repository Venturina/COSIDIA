#ifndef _DEBUG_CLOCK_HPP_SDNFO
#define _DEBUG_CLOCK_HPP_SDNFO

#include "core/SteadyClock.hpp"

namespace cosidia
{

class DebugClock : public SteadyClock
{
public:
    DebugClock(float simSpeed) : SteadyClock(simSpeed) {};

    virtual SteadyClock::time_point getRealTimeForCurrentSimTime() override;
    virtual SimClock::time_point getSimTimeNow() const override;
    virtual SteadyClock::duration getDurationUntil(SimClock::time_point) override;
    virtual SteadyClock::time_point getTimePointforSimTime(SimClock::time_point) override;
};

} // namespace cosidia

#endif /* _DEBUG_CLOCK_HPP_SDNFO */
