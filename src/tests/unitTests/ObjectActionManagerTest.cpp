//#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>

#include "core/DurationAction.hpp"
#include "objects/ObjectActionManager.hpp"

using namespace cosidia;

TEST_CASE( "Test the action manager", "[Action Manager]" ) {
    std::shared_ptr<DurationAction> a0 = ActionFactory<DurationAction>::create(std::chrono::milliseconds(1), SimClock::atMillisecond(1), ObjectId { 1 }, ObjectId { 5 });
    std::shared_ptr<DurationAction> a1 = ActionFactory<DurationAction>::create(std::chrono::milliseconds(1), SimClock::atMillisecond(1), ObjectId { 1 }, ObjectId { 5 });
    std::shared_ptr<DurationAction> a2 = ActionFactory<DurationAction>::create(std::chrono::milliseconds(1), SimClock::atMillisecond(1), ObjectId { 1 }, ObjectId { 5 });
    std::shared_ptr<DurationAction> a3 = ActionFactory<DurationAction>::create(std::chrono::milliseconds(1), SimClock::atMillisecond(1), ObjectId { 1 }, ObjectId { 5 });
    std::shared_ptr<DurationAction> a4 = ActionFactory<DurationAction>::create(std::chrono::milliseconds(1), SimClock::atMillisecond(1), ObjectId { 1 }, ObjectId { 5 });
    std::shared_ptr<DurationAction> a5 = ActionFactory<DurationAction>::create(std::chrono::milliseconds(1), SimClock::atMillisecond(1), ObjectId { 1 }, ObjectId { 5 });


    ObjectActionManager manager;

    SECTION( "By default, no action is available" ) {
        REQUIRE_FALSE(manager.isActionAvailable());
    }

    SECTION( "One action running" ) {
        REQUIRE_FALSE(manager.isActionAvailable());
        REQUIRE_FALSE(manager.startOrDelay(a0));
        REQUIRE_FALSE(manager.isActionAvailable());
        REQUIRE_FALSE(manager.endAndCheckAvailable());
    }

    SECTION("One action waiting") {
        REQUIRE_FALSE(manager.isActionAvailable());
        REQUIRE_FALSE(manager.startOrDelay(a0));
        REQUIRE(manager.startOrDelay(a1));
        REQUIRE(manager.endAndCheckAvailable());
    }

    SECTION("Pop one waiting action") {
        REQUIRE_FALSE(manager.isActionAvailable());
        REQUIRE_FALSE(manager.startOrDelay(a0));
        REQUIRE(manager.startOrDelay(a1));
        REQUIRE(manager.endAndCheckAvailable()); // ends a0
        REQUIRE(&manager.fetchNextAction() == a1.get());
        REQUIRE(manager.isActionAvailable());  // a1 still available
        REQUIRE(manager.activateNextAvailableAction() == a1);  // starts a1 and returns pointer to a1
    }

    SECTION("Try a sequence of poping and starting") {
        REQUIRE_FALSE(manager.isActionAvailable());
        REQUIRE_FALSE(manager.startOrDelay(a0));
        REQUIRE(manager.startOrDelay(a1));
        REQUIRE(manager.startOrDelay(a2));
        REQUIRE(manager.endAndCheckAvailable());                // ends execution of a0
        REQUIRE(&manager.fetchNextAction() == a1.get());
        REQUIRE(manager.activateNextAvailableAction() == a1);   // starts a1
        REQUIRE(manager.isActionAvailable());                   // a2 still waiting
        REQUIRE(manager.endAndCheckAvailable());                // ends a1
        REQUIRE(&manager.fetchNextAction() == a2.get());        // just a fetch, nothing else
        REQUIRE(manager.isActionAvailable());                   // a2 still on list, waiting
        REQUIRE(manager.activateNextAvailableAction() == a2);   // a2 started
        REQUIRE_FALSE(manager.isActionAvailable());             // a2 running, nothing else waiting
        REQUIRE(manager.startOrDelay(a3));                      // a3 delayed because a2 still running
        REQUIRE(manager.isActionAvailable());                   // a3 waiting
        REQUIRE(manager.startOrDelay(a4));                      // a3 and a4 waiting
        REQUIRE(&manager.fetchNextAction() == a3.get());        // a3 in front
        REQUIRE(manager.endAndCheckAvailable());                // a2 ended
        REQUIRE(manager.endAndCheckAvailable());                // nothing should change, a3 still waiting
        REQUIRE(&manager.fetchNextAction() == a3.get());        // a3 still in front
        REQUIRE(manager.activateNextAvailableAction() == a3);   // a3 started
        REQUIRE_THROWS(manager.activateNextAvailableAction());  // should throw an runtime error, a3 must be ended
        REQUIRE(&manager.fetchNextAction() == a4.get());        // a4 in front
        REQUIRE(manager.endAndCheckAvailable());                // a3 ended
        REQUIRE(manager.activateNextAvailableAction() == a4);   // a4 running
    }

}