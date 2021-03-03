#include <catch2/catch.hpp>
#include <trompeloeil/include/catch2/trompeloeil.hpp>

#include "core/ActionList.hpp"
#include "core/Core.hpp"
#include "debug/DebugAction.hpp"

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
    };

};

class DebugActionMap : public ActionList
{
public:
    HandlerMap* getHandlerMap() { return mHandlerMap.get(); }
};


TEST_CASE("Action List", "[ActionList]") {
    MockCore core;
    std::shared_ptr<debug::DebugDurationAction> a1 = ActionFactory<debug::DebugDurationAction>::create(std::chrono::milliseconds(1), SimClock::atMillisecond(1), ObjectId { 1 }, ObjectId { 5 });
    std::shared_ptr<debug::DebugDurationAction> a2 = ActionFactory<debug::DebugDurationAction>::create(std::chrono::milliseconds(1), SimClock::atMillisecond(3), ObjectId { 1 }, ObjectId { 5 });
    std::shared_ptr<debug::DebugDurationAction> a3 = ActionFactory<debug::DebugDurationAction>::create(std::chrono::milliseconds(1), SimClock::atMillisecond(2), ObjectId { 1 }, ObjectId { 5 });

    // a1s -> 1, a1e -> 2
    // a2s -> 3, a2e -> 4
    // a3s -> 2, a3e -> 3

    DebugActionMap list;

    SECTION("Insert and Get") {
        // insert actions
        list.insertHandler(a1->getStartHandler()); // time = 1, a1s
        REQUIRE(list.getHandlerMap()->size() == 1);
        list.insertHandler(a2->getStartHandler()); // time = 3, a2s
        REQUIRE(list.getHandlerMap()->size() == 2);
        list.insertHandler(a1->getEndHandler()); // time = 2, a1e
        REQUIRE(list.getHandlerMap()->size() == 3);
        list.insertHandler(a3->getStartHandler()); // time = 2, a3s
        REQUIRE(list.getHandlerMap()->size() == 3);
        list.insertHandler(a2->getEndHandler()); // time = 4, a2e
        REQUIRE(list.getHandlerMap()->size() == 4);
        list.insertHandler(a3->getEndHandler()); // time = 3, a3e
        REQUIRE(list.getHandlerMap()->size() == 4);
        // list layout
        /*  Time    Handlers
            [1]     a1s
            [2]     a1e, a3s,
            [3]     a2s, a3e,
            [4]     a2e
        */


        // pop and get
        REQUIRE(*(list.getNextHandlerList().begin()) == a1->getStartHandler());
        REQUIRE(list.getNextTimePoint() == a1->getStartTime());
        REQUIRE(list.getHandlerMap()->size() == 4);

        auto handlers = list.popNextHandlers();
        REQUIRE(handlers.size() == 1);
        REQUIRE(*(handlers.begin()) == a1->getStartHandler() );
        REQUIRE(list.getHandlerMap()->size() == 3);
        /*  Time    Actions
            [2]     a1e, a3s,
            [3]     a2s, a3e,
            [4]     a2e
        */


        REQUIRE(*(list.getNextHandlerList().begin()) == a1->getEndHandler());
        REQUIRE(list.getNextHandlerList().size() == 2);
        REQUIRE(list.getNextTimePoint() == a1->getEndTime());
        REQUIRE(list.getHandlerMap()->size() == 3);
        /*  Time    Actions
            [2]     a1e, a3s,
            [3]     a2s, a3e,
            [4]     a2e
        */

        handlers = list.popNextHandlers();
        REQUIRE(handlers.size() == 2);
        REQUIRE(list.getHandlerMap()->size() == 2);
        REQUIRE(*(list.getNextHandlerList().begin()) == a2->getStartHandler());
        /*  Time    Actions
            [3]     a2s, a3e,
            [4]     a2e
        */

        // insert again
        list.insertHandler(a1->getStartHandler());
        REQUIRE(*(list.getNextHandlerList().begin()) == a1->getStartHandler());
        REQUIRE(list.getNextTimePoint() == a1->getStartTime());
        REQUIRE(list.getHandlerMap()->size() == 3);
        /*  Time    Actions
            [1]     a1s
            [3]     a2s, a3e,
            [4]     a2e
        */
    }

    SECTION("Remove") {
        list.insertHandler(a1->getStartHandler()); // time = 1, a1s
        REQUIRE(list.getHandlerMap()->size() == 1);
        list.insertHandler(a2->getStartHandler()); // time = 3, a2s
        REQUIRE(list.getHandlerMap()->size() == 2);
        list.insertHandler(a1->getEndHandler()); // time = 2, a1e
        REQUIRE(list.getHandlerMap()->size() == 3);
        list.insertHandler(a3->getStartHandler()); // time = 2, a3s
        REQUIRE(list.getHandlerMap()->size() == 3);
        list.insertHandler(a2->getEndHandler()); // time = 4, a2e
        REQUIRE(list.getHandlerMap()->size() == 4);
        list.insertHandler(a3->getEndHandler()); // time = 3, a3e
        REQUIRE(list.getHandlerMap()->size() == 4);
        // list layout
        /*  Time    Handlers
            [1]     a1s
            [2]     a1e, a3s,
            [3]     a2s, a3e,
            [4]     a2e
        */

        // Removing Handlers is not yet implemented
        #ifdef COSIDIA_SAFE
        REQUIRE_THROWS(list.removeHandler(a1->getStartHandler(), a1->getStartTime()));
        #endif
    }



    SECTION("check order") {
        list.insertHandler(a1->getStartHandler()); // time = 1, a1s
        list.insertHandler(a2->getStartHandler()); // time = 3, a2s
        list.insertHandler(a1->getEndHandler()); // time = 2, a1e
        list.insertHandler(a3->getStartHandler()); // time = 2, a3s
        list.insertHandler(a2->getEndHandler()); // time = 4, a2e
        list.insertHandler(a3->getEndHandler()); // time = 3, a3e
        // list layout
        /*  Time    Handlers
            [1]     a1s
            [2]     a1e, a3s,
            [3]     a2s, a3e,
            [4]     a2e
        */

        auto pop1 = list.popNextHandlers();
        auto pop2 = list.popNextHandlers();
        auto pop3 = list.popNextHandlers();
        auto pop4 = list.popNextHandlers();

        REQUIRE(pop1.size() == 1);
        REQUIRE(*(pop1.begin()) == a1->getStartHandler());
        REQUIRE(pop2.size() == 2);
        REQUIRE(*(pop2.begin()) == a1->getEndHandler());
        REQUIRE(*(++pop2.begin()) == a3->getStartHandler());
        REQUIRE(pop3.size() == 2);
        REQUIRE(*(pop3.begin()) == a2->getStartHandler());
        REQUIRE(*(++pop3.begin()) == a3->getEndHandler());
        REQUIRE(pop4.size() == 1);
        REQUIRE(*(pop4.begin()) == a2->getEndHandler());

        REQUIRE(list.getHandlerMap()->size() == 0);
    }

}
