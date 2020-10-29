#include <catch2/catch.hpp>
#include <trompeloeil/include/catch2/trompeloeil.hpp>

#include "debug/DebugObject.hpp"
#include "core/Action.hpp"
#include "core/Core.hpp"
#include "objects/ObjectActionManager.hpp"



using namespace paresis;

class MockObjectActionManager : public ObjectActionManager
{
public:
    MAKE_MOCK1(startOrDelay, bool(std::shared_ptr<Action>), override);
};

class MockClock : public SteadyClock
{
public:
    MockClock() : SteadyClock(1) {};
    MAKE_CONST_MOCK0(getSimTimeNow, std::chrono::nanoseconds(), override);
};

class MockCore : public Core
{
public:
    MockCore() : Core() {
        mClock.reset(new MockClock());
    };

    MAKE_MOCK1(scheduleAction, void(std::shared_ptr<Action>), override);
};

class MockDebugObjectChild : public DebugObjectChild
{
public:
    MockDebugObjectChild() : DebugObjectChild() {
        mActionManager.reset(new MockObjectActionManager);
    }
    MAKE_MOCK1(startExecution, void(std::shared_ptr<Action>), override);
    MAKE_MOCK1(endExecution, void(std::shared_ptr<Action>), override);
    MAKE_MOCK1(initObject, void(std::shared_ptr<Action>), override);

    void mockInit(std::shared_ptr<Action> a) {DebugObjectChild::initObject(a);}
    void mockInit() {DebugObjectChild::initObject();}

    ObjectActionManager* getMockedObjectManager() { return mActionManager.get(); }
};

TEST_CASE( "BaseObject Test: Init and SelfAction", "[BaseObject]" )
{
    std::shared_ptr<MockDebugObjectChild> child(new MockDebugObjectChild());
    std::shared_ptr<DebugObject> parent(new DebugObjectParent());

    std::shared_ptr<Action> a1(new Action(std::chrono::milliseconds(1), Action::Kind::START, std::chrono::milliseconds(500), 1, 5));
    std::shared_ptr<Action> a2(new Action(std::chrono::milliseconds(1), Action::Kind::START, std::chrono::milliseconds(1), 2, 5));

    SECTION("Create self Action") {
        REQUIRE_FALSE(child->isInitialized());
        REQUIRE_FALSE(parent->isInitialized());

        child->mockInit(a1);
        parent->initObject(a2);

        REQUIRE(child->isInitialized());
        REQUIRE(parent->isInitialized());

        auto self = child->testCreateSelfAction(std::chrono::milliseconds(2), std::chrono::milliseconds(1));
        REQUIRE(self->getAffected().front() == child->getObjectId());
        REQUIRE(self->getKind() == Action::Kind::START);
        REQUIRE(self->getStartTime() == std::chrono::milliseconds(1));
        REQUIRE(self->getDuration() == std::chrono::milliseconds(2));
        REQUIRE(self->getActionData() == nullptr);
    }

    #ifdef PARESIS_SAFE
    SECTION("Is Initialized") {
            REQUIRE_THROWS(parent->execute(a1));
    }
    #endif
}

TEST_CASE( "BaseObject Test: Execution", "[BaseObject]" )
{
    {
        MockCore* c = new MockCore();
        std::shared_ptr<MockDebugObjectChild> child(new MockDebugObjectChild());
        auto mockClock = dynamic_cast<const MockClock*>(&(c->getClock()));
        auto mockActionManager = dynamic_cast<MockObjectActionManager*>(child->getMockedObjectManager());

        std::shared_ptr<Action> a1(new Action(std::chrono::milliseconds(1), Action::Kind::START, std::chrono::milliseconds(500), 1, 5));
        std::shared_ptr<Action> a2(new Action(std::chrono::milliseconds(1), Action::Kind::END, std::chrono::milliseconds(1), 2, 5));
        a1->setActionId(5);
        a2->setBeginId(a1->getActionId());
        std::shared_ptr<Action> a3(new Action(std::chrono::milliseconds(1), Action::Kind::INIT, std::chrono::milliseconds(1), 2, 5));


        REQUIRE_CALL(*mockActionManager, startOrDelay(a1))      // call_1
                    .RETURN(false);

        /** just for refference
        REQUIRE_CALL(*c, scheduleAction(trompeloeil::_))        // call_2
                    .WITH(std::shared_ptr<Action>(_1)->getKind() == Action::Kind::END)
                    .WITH(std::shared_ptr<Action>(_1)->getStartTime() == (a1->getStartTime() + a1->getDuration()));

        **/
        REQUIRE_CALL(*child, startExecution(a1));               // call_3

        child->mockInit();                                      // initializes object properly
        REQUIRE(child->execute(a1)== child->getObjectId());     // enforces call_1

        REQUIRE_CALL(*child, endExecution(a2));
        ALLOW_CALL(*mockClock, getSimTimeNow())
                    .RETURN(a1->getStartTime());
        child->execute(a2);

        REQUIRE_CALL(*child, initObject(a3));
        child->execute(a3);
    }
}