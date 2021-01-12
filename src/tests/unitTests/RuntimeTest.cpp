#include <catch2/catch.hpp>
#include "core/Core.hpp"
#include "core/SteadyClock.hpp"
#include "networking/Runtime.hpp"
#include "networking/VanetzaDefs.hpp"
#include "vanetza/geonet/mib.hpp"
#include "vanetza/geonet/router.hpp"

#include <chrono>

using namespace cosidia;

class MockClock : public SteadyClock
{
public:
    MockClock() : SteadyClock(1) {};
};

class MockCore : public Core
{
public:
    MockCore() : Core() {
        mClock.reset(new MockClock());
        setUtcStartTime();
    };

};

TEST_CASE( "Runtime", "[Runtime]" ) {
    MockCore core;
    // time point 100ms after simulation start
    SteadyClock::duration d{std::chrono::milliseconds(100)};

    auto runtime = Runtime::makeRuntime(d);
    vanetza::geonet::MIB mib;
    vanetza::geonet::Router router {*runtime, mib};  // start router at 100ms
    auto runtimeStart = runtime->now();
    auto simulationStart = runtime->now() - std::chrono::milliseconds(100);
    auto simStart2 = getUtcStartTime().time_since_epoch() - utcItsDiff;

    REQUIRE(std::chrono::duration_cast<std::chrono::microseconds>(simStart2) == simulationStart.time_since_epoch());

    // absolute time to next action
    REQUIRE(runtime->getDurationStartToNext() == std::chrono::milliseconds(100));

    // from now to next action
    REQUIRE(runtime->getDurationNowToNext() == std::chrono::milliseconds(0));

    runtime->triggerAbsolute(d);
    REQUIRE(runtime->now() - runtimeStart == std::chrono::milliseconds(0));

    REQUIRE(runtime->getDurationStartToNext() == std::chrono::microseconds(3798856));
    REQUIRE(runtime->now() - runtimeStart == std::chrono::microseconds(0));
    REQUIRE(runtime->getDurationNowToNext() == std::chrono::microseconds(3698856));

    // set now to 1us before next event
    runtime->triggerAbsolute(runtime->getDurationStartToNext() - std::chrono::microseconds(1));

    REQUIRE(runtime->getDurationStartToNext() == std::chrono::microseconds(3798856));
    REQUIRE(runtime->now() - simulationStart == std::chrono::microseconds(3798855));  //startToNext - 1us
    REQUIRE(runtime->now() - runtimeStart == std::chrono::microseconds(3698855));
    REQUIRE(runtime->next() - runtime->now() == std::chrono::microseconds(1));
    REQUIRE(std::chrono::duration_cast<std::chrono::microseconds>(runtime->getDurationNowToNext()) == std::chrono::microseconds(1));  // same as next - now

    // advance 1us to trigger next event
    runtime->triggerAbsolute(runtime->getDurationStartToNext());

    REQUIRE(runtime->now() - simulationStart == std::chrono::microseconds(3798856)); // 1
    REQUIRE(runtime->getDurationStartToNext() == std::chrono::microseconds(7519689)); // 2
    REQUIRE(runtime->getDurationNowToNext() == std::chrono::microseconds(3720833)); // = 1-2

}