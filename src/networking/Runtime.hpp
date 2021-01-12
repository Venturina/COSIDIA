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
    explicit Runtime(vanetza::Clock::time_point init, vanetza::Clock::duration);

    std::chrono::nanoseconds getDurationStartToNext() const;

    std::chrono::nanoseconds getDurationNowToNext() const;

    void triggerAbsolute(vanetza::Clock::duration);
    void triggerAbsolute(SteadyClock::duration);

    static std::unique_ptr<Runtime> makeRuntime(SteadyClock::duration initTime);

private:
    vanetza::Clock::duration mLastTrigger;
    vanetza::Clock::duration mRouterStartSimTime;
    vanetza::Clock::time_point mSimulationStartTime;
    vanetza::Clock::time_point mRouterStartTime;
};

} // namespace cosidia
