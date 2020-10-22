#include "core/Core.hpp"
#include "networking/ParesisRuntime.hpp"
#include "networking/VanetzaDefs.hpp"

#include "loguru/loguru.hpp"


namespace paresis
{

ParesisRuntime::ParesisRuntime(vanetza::Clock::time_point init, vanetza::Clock::duration initSim) : vanetza::ManualRuntime(init)
{
    vanetza::Clock::time_point tp {std::chrono::duration_cast<vanetza::Clock::duration>(getUtcStartTime().time_since_epoch() - utcItsDiff)};
    mSimulationStartTime = tp;
    mRouterStartTime = init;
    mRouterStartSimTime = initSim;
    mLastTrigger = std::chrono::milliseconds{0};
}

std::chrono::nanoseconds ParesisRuntime::getDurationStartToNext() const
{
    auto duration = next() - mSimulationStartTime;
    DLOG_F(ERROR, "duration from start to next: %d", std::chrono::duration_cast<std::chrono::milliseconds>(duration).count());
    return duration;
}

// void ParesisRuntime::trigger(vanetza::Clock::duration relativeDuration)
// {
//     mLastTrigger += relativeDuration;
//     ManualRuntime::trigger(relativeDuration);
// }

void ParesisRuntime::triggerAbsolute(vanetza::Clock::duration absoluteDuration)
{
    auto advance = absoluteDuration - mRouterStartSimTime;
    DLOG_F(ERROR, "current router time: %d", std::chrono::duration_cast<std::chrono::milliseconds>(advance).count());
    trigger(mRouterStartTime+(absoluteDuration-mRouterStartSimTime));
    mLastTrigger = absoluteDuration;
}

} // namespace paresis
