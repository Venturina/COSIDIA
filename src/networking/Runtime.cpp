#include "core/Core.hpp"
#include "networking/Runtime.hpp"
#include "networking/VanetzaDefs.hpp"
#include "utils/enforce.hpp"
#include "utils/invariant.hpp"

#include "loguru/loguru.hpp"


namespace cosidia
{

vanetza::Clock::time_point Runtime::convert(SimClock::time_point sim)
{
    // getUtcStartTime reports the UTC time point corresponding to the start of simulation
    std::chrono::system_clock::time_point utc = getUtcStartTime() + sim.time_since_epoch();
    // convert UTC (UNIX time epoch) to ITS (TAI) clock starting at 2004-01-01
    vanetza::Clock::time_point its {
        std::chrono::duration_cast<vanetza::Clock::duration>(utc.time_since_epoch() - utcItsDiff)
    };
    return its;
}

Runtime::Runtime(SimClock::time_point init) : vanetza::ManualRuntime(convert(init))
{
    mSimStartTime = init;
    mItsStartTime = now();
}

SimClock::time_point Runtime::getNextStart() const
{
    return mSimStartTime + (next() - mItsStartTime);
}

SimClock::duration Runtime::getDurationNowToNext() const
{
    return next() - now();
}

void Runtime::trigger(vanetza::Clock::time_point tp)
{
    enforce(tp >= mLastTrigger, "trigger can only advance forward in time");
    auto advance = tp - mItsStartTime;
    DLOG_F(INFO, "current router run for: %d us", std::chrono::duration_cast<std::chrono::microseconds>(advance).count());
    ManualRuntime::trigger(tp);
    mLastTrigger = tp;
}

void Runtime::trigger(SimClock::time_point tp)
{
    SimClock::duration simAdvance = tp - mSimStartTime;
    auto itsAdvance = std::chrono::duration_cast<vanetza::Clock::duration>(simAdvance);
    vanetza::Clock::time_point itsAdvanceTo = mItsStartTime + itsAdvance;
    invariant(itsAdvanceTo >= mLastTrigger, "trigger can only advance forward in time");
    trigger(itsAdvanceTo);
}

std::unique_ptr<Runtime> Runtime::makeRuntime(SimClock::time_point initTime)
{
    return std::make_unique<Runtime>(initTime);
}

} // namespace cosidia
