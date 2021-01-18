#include <catch2/catch.hpp>
#include <trompeloeil/include/catch2/trompeloeil.hpp>

#include "core/ActionList.hpp"
#include "core/Core.hpp"


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
    ActionMap* getActionMap() { return mActionMap.get(); }
};


TEST_CASE("Action List", "[ActionList]") {
    MockCore core;
    std::shared_ptr<Action> a1(new Action(std::chrono::milliseconds(1), Action::Kind::START, SimClock::atMillisecond(1), ObjectId { 1 }, ObjectId { 5 }));
    std::shared_ptr<Action> a2(new Action(std::chrono::milliseconds(1), Action::Kind::START, SimClock::atMillisecond(2), ObjectId { 2 }, ObjectId { 5 }));
    std::shared_ptr<Action> a3(new Action(std::chrono::milliseconds(1), Action::Kind::START, SimClock::atMillisecond(2), ObjectId { 3 }, ObjectId { 5 }));
    std::shared_ptr<Action> a4(new Action(std::chrono::milliseconds(1), Action::Kind::START, SimClock::atMillisecond(3), ObjectId { 4 }, ObjectId { 5 }));
    std::shared_ptr<Action> a5(new Action(std::chrono::milliseconds(1), Action::Kind::START, SimClock::atMillisecond(2), ObjectId { 5 }, ObjectId { 5 }));
    std::shared_ptr<Action> a6(new Action(std::chrono::milliseconds(1), Action::Kind::START, SimClock::atMillisecond(3), ObjectId { 6 }, ObjectId { 5 }));

    DebugActionMap list;

    SECTION("Insert and Get") {
        // insert actions
        list.insertAction(a1); // start time = 1
        REQUIRE(list.getActionMap()->size() == 1);
        list.insertAction(a4); // start time = 3
        REQUIRE(list.getActionMap()->size() == 2);
        list.insertAction(a5); // start time = 2
        REQUIRE(list.getActionMap()->size() == 3);
        list.insertAction(a2); // start time = 2
        REQUIRE(list.getActionMap()->size() == 4);
        list.insertAction(a3); // start time = 2
        REQUIRE(list.getActionMap()->size() == 5);
        list.insertAction(a6); // start time = 3
        REQUIRE(list.getActionMap()->size() == 6);

        // pop and get
        REQUIRE(list.getActionMap()->begin()->second == a1);
        REQUIRE(list.getNextAction() == a1);
        REQUIRE(list.getActionMap()->size() == 6);
        REQUIRE(list.popNextAction() == a1);
        REQUIRE(list.getActionMap()->size() == 5);

        // insert again
        list.insertAction(a1);
        REQUIRE(list.getActionMap()->begin()->second == a1);
        REQUIRE(list.getNextAction() == a1);
        REQUIRE(list.getActionMap()->size() == 6);
    }

    SECTION("Remove") {
        // insert actions
        list.insertAction(a1); // start time = 1
        list.insertAction(a4); // start time = 3
        list.insertAction(a5); // start time = 2
        list.insertAction(a2); // start time = 2
        list.insertAction(a3); // start time = 2
        list.insertAction(a6); // start time = 3

        // wrong time hint
        REQUIRE(list.removeAction(a3, SimClock::atMillisecond(1)) == false);
        REQUIRE(list.getActionMap()->size() == 6);

        // actually remove
        REQUIRE(list.removeAction(a3, SimClock::atMillisecond(2)) == true);
        REQUIRE(list.getActionMap()->size() == 5);

        // wrong pointer
        REQUIRE(list.removeAction(a3, SimClock::atMillisecond(2)) == false);
        REQUIRE(list.getActionMap()->size() == 5);
    }

    SECTION("check order") {
        list.insertAction(a1); // start time = 1
        list.insertAction(a4); // start time = 3
        list.insertAction(a5); // start time = 2
        list.insertAction(a2); // start time = 2
        list.insertAction(a3); // start time = 2
        list.insertAction(a6); // start time = 3
        REQUIRE(list.removeAction(a3, SimClock::atMillisecond(2)) == true);


        auto pop1 = list.popNextAction();
        auto pop2 = list.popNextAction();
        auto pop3 = list.popNextAction();
        auto pop4 = list.popNextAction();
        auto pop5 = list.popNextAction();

        // if key is equal, new inserted item will placed behind all items with same key
        REQUIRE(pop1 == a1);
        REQUIRE(pop2 == a5);
        REQUIRE(pop3 == a2);
        REQUIRE(pop4 == a4);
        REQUIRE(pop5 == a6);
        REQUIRE(list.getActionMap()->size() == 0);

        list.insertAction(a1); // start time = 1
        REQUIRE(list.getActionMap()->size() == 1);
        list.insertAction(a3); // start time = 2
        REQUIRE(list.getActionMap()->size() == 2);
        list.insertAction(a4); // start time = 3
        REQUIRE(list.getActionMap()->size() == 3);
        list.insertAction(a5); // start time = 2
        REQUIRE(list.getActionMap()->size() == 4);
        list.insertAction(a2); // start time = 2
        REQUIRE(list.getActionMap()->size() == 5);
        list.insertAction(a6); // start time = 3
        REQUIRE(list.getActionMap()->size() == 6);

        pop1 = list.popNextAction();
        pop2 = list.popNextAction();
        pop3 = list.popNextAction();
        pop4 = list.popNextAction();
        pop5 = list.popNextAction();
        auto pop6 = list.popNextAction();

        REQUIRE(pop1 == a1);
        REQUIRE(pop2 == a3);
        REQUIRE(pop3 == a5);
        REQUIRE(pop4 == a2);
        REQUIRE(pop5 == a4);
        REQUIRE(pop6 == a6);
        REQUIRE(list.getActionMap()->size() == 0);
    }
}