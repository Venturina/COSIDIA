#pragma once


#include "vanetza/common/manual_runtime.hpp"

namespace paresis
{

class ParesisRuntime : public vanetza::ManualRuntime
{
public:
    /**
     * Create runtime
     * \param init initialization value of internal clock
     */
    explicit ParesisRuntime(vanetza::Clock::time_point init, vanetza::Clock::duration);

    std::chrono::nanoseconds getDurationStartToNext() const;

    void triggerAbsolute(vanetza::Clock::duration);

    //void trigger(std::chrono::duration) override;

private:
    vanetza::Clock::duration mLastTrigger;
    vanetza::Clock::duration mRouterStartSimTime;
    vanetza::Clock::time_point mSimulationStartTime;
    vanetza::Clock::time_point mRouterStartTime;
};

} // namespace paresis
