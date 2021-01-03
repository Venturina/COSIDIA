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
    DLOG_F(INFO, "duration from start to next: %d us", std::chrono::duration_cast<std::chrono::microseconds>(duration).count());
    return duration;
}

std::chrono::nanoseconds ParesisRuntime::getDurationNowToNext() const
{
    auto duration = next() - now();
    DLOG_F(INFO, "duration from now to next: %d us", std::chrono::duration_cast<std::chrono::microseconds>(duration).count());
    return duration;
}

void ParesisRuntime::triggerAbsolute(vanetza::Clock::duration absoluteDuration)
{
    auto advance = absoluteDuration - mRouterStartSimTime;
    DLOG_F(INFO, "current router time: %d us", std::chrono::duration_cast<std::chrono::microseconds>(advance).count());
    trigger(mRouterStartTime+(absoluteDuration-mRouterStartSimTime));
    mLastTrigger = absoluteDuration;
}

void ParesisRuntime::triggerAbsolute(SteadyClock::duration absoluteSteadyClockDuration)
{
    auto absoluteDuration = std::chrono::duration_cast<vanetza::Clock::duration>(absoluteSteadyClockDuration);
    auto advance = absoluteDuration - mRouterStartSimTime;
    DLOG_F(INFO, "current router time: %d us", std::chrono::duration_cast<std::chrono::microseconds>(advance).count());
    trigger(mRouterStartTime+(absoluteDuration-mRouterStartSimTime));
    mLastTrigger = absoluteDuration;
}

std::unique_ptr<ParesisRuntime> ParesisRuntime::makeRuntime(SteadyClock::duration initTime) {
    vanetza::Clock::time_point ts { std::chrono::duration_cast<vanetza::Clock::duration>(getUtcStartTime().time_since_epoch() - utcItsDiff + initTime)};
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(initTime);
    return std::make_unique<ParesisRuntime>(ts, duration);
}

} // namespace paresis
