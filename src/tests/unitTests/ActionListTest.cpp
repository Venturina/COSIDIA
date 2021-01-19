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
        REQUIRE(list.getActionMap()->size() == 3);
        list.insertAction(a3); // start time = 2
        REQUIRE(list.getActionMap()->size() == 3);
        list.insertAction(a6); // start time = 3
        REQUIRE(list.getActionMap()->size() == 3);
        // list layout
        /*  Time    Actions
            [1]     a1
            [2]     a5, a2, a3
            [3]     a4, a6
        */


        // pop and get
        REQUIRE(*(list.getNextActionList().begin()) == a1);
        REQUIRE(list.getNextTimePoint() == a1->getStartTime());
        REQUIRE(list.getActionMap()->size() == 3);

        auto actions = list.popNextActions();
        REQUIRE(actions.size() == 1);
        REQUIRE(*(actions.begin()) == a1 );
        REQUIRE(list.getActionMap()->size() == 2);
        /*  Time    Actions
            [2]     a5, a2, a3
            [3]     a4, a6
        */


        REQUIRE(*(list.getNextActionList().begin()) == a5);
        REQUIRE(list.getNextActionList().size() == 3);
        REQUIRE(list.getNextTimePoint() == a5->getStartTime());
        REQUIRE(list.getActionMap()->size() == 2);
        /*  Time    Actions
            [2]     a5, a2, a3
            [3]     a4, a6
        */

        actions = list.popNextActions();
        REQUIRE(actions.size() == 3);
        REQUIRE(list.getActionMap()->size() == 1);
        /*  Time    Actions
            [3]     a4, a6
        */

        // insert again
        list.insertAction(a1);
        REQUIRE(*(list.getNextActionList().begin()) == a1);
        REQUIRE(list.getNextTimePoint() == a1->getStartTime());
        REQUIRE(list.getActionMap()->size() == 2);
        /*  Time    Actions
            [1]     a1
            [3]     a4, a6
        */
    }

    SECTION("Remove") {
        // insert actions
        list.insertAction(a1); // start time = 1
        list.insertAction(a4); // start time = 3
        list.insertAction(a5); // start time = 2
        list.insertAction(a2); // start time = 2
        list.insertAction(a3); // start time = 2
        list.insertAction(a6); // start time = 3
        // list layout
        /*  Time    Actions
            [1]     a1
            [2]     a5, a2, a3
            [3]     a4, a6
        */

        // wrong time hint
        REQUIRE(list.removeAction(a3, SimClock::atMillisecond(1)) == false);
        REQUIRE(list.getActionMap()->size() == 3);

        // actually remove
        REQUIRE(list.removeAction(a3, SimClock::atMillisecond(2)) == true);
        REQUIRE(list.getActionMap()->size() == 3);
        /*  Time    Actions
            [1]     a1
            [2]     a5, a2
            [3]     a4, a6
        */

        // wrong pointer
        REQUIRE(list.removeAction(a3, SimClock::atMillisecond(2)) == false);
        REQUIRE(list.getActionMap()->size() == 3);

        // remove with FAS
        REQUIRE(list.removeAction(a1, SimClock::atMillisecond(1)) == true);
        REQUIRE(list.getActionMap()->size() == 2);
        /*  Time    Actions
            [2]     a5, a2
            [3]     a4, a6
        */

        auto l = list.getNextActionList();
        REQUIRE(l.size() == 2);
        auto lIterator = l.begin();
        REQUIRE(lIterator->get() == a5.get());
        lIterator++;
        REQUIRE(lIterator->get() == a2.get());

        // insert again
        list.insertAction(a1);
        REQUIRE(list.getActionMap()->size() == 3);
        /*  Time    Actions
            [1]     a1
            [2]     a5, a2
            [3]     a4, a6
        */

        REQUIRE(list.removeAction(a5, SimClock::atMillisecond(2)) == true);
        REQUIRE(list.getActionMap()->size() == 3);
        REQUIRE(list.removeAction(a2, SimClock::atMillisecond(2)) == true);
        REQUIRE(list.getActionMap()->size() == 2);
        /*  Time    Actions
            [1]     a1
            [3]     a4, a6
        */
        
    }



    SECTION("check order") {
        list.insertAction(a1); // start time = 1
        list.insertAction(a4); // start time = 3
        list.insertAction(a5); // start time = 2
        list.insertAction(a2); // start time = 2
        list.insertAction(a3); // start time = 2
        list.insertAction(a6); // start time = 3
        /*  Time    Actions
            [1]     a1
            [2]     a5, a2, a3
            [3]     a4, a6
        */

        REQUIRE(list.removeAction(a2, SimClock::atMillisecond(2)) == true);
        /*  Time    Actions
            [1]     a1
            [2]     a5, a3
            [3]     a4, a6
        */

        auto pop1 = list.popNextActions();
        auto pop2 = list.popNextActions();
        auto pop3 = list.popNextActions();

        REQUIRE(pop1.size() == 1);
        REQUIRE(*(pop1.begin()) == a1);
        REQUIRE(pop2.size() == 2);
        REQUIRE(*(pop2.begin()) == a5);
        REQUIRE(*(++pop2.begin()) == a3);
        REQUIRE(pop3.size() == 2);
        REQUIRE(*(pop3.begin()) == a4);
        REQUIRE(*(++pop3.begin()) == a6);

        REQUIRE(list.getActionMap()->size() == 0);
    }
}
