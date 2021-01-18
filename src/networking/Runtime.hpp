#pragma once


#include "vanetza/common/manual_runtime.hpp"

namespace cosidia
{

class Runtime : public vanetza::ManualRuntime
{
public:
    /**
     * Create runtime
     * \param init initialization value of internal clock
     */
    explicit Runtime(SimClock::time_point start);

    SimClock::time_point getNextStart() const;

    SimClock::duration getDurationNowToNext() const;

    void trigger(vanetza::Clock::time_point);
    void trigger(SimClock::time_point);

    static std::unique_ptr<Runtime> makeRuntime(SimClock::time_point initTime);

    static vanetza::Clock::time_point convert(SimClock::time_point);

private:
    SimClock::time_point mSimStartTime;
    vanetza::Clock::time_point mItsStartTime;
    vanetza::Clock::time_point mLastTrigger;
};

} // namespace cosidia
