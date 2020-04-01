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
    MAKE_MOCK0(getSimTimeNow, std::chrono::nanoseconds(), override);
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

    ObjectActionManager* getMockedObjectManager() { return mActionManager.get(); }
};

TEST_CASE( "BaseObject Test: Init and SelfAction", "[BaseObject]" )
{
    // MockCore c;
    // setCoreP(&c);
    std::shared_ptr<MockDebugObjectChild> child(new MockDebugObjectChild());
    std::shared_ptr<DebugObject> parent(new DebugObjectParent());

    std::shared_ptr<Action> a1(new Action(std::chrono::milliseconds(1), Action::Kind::START, std::chrono::milliseconds(500), 1));
    std::shared_ptr<Action> a2(new Action(std::chrono::milliseconds(1), Action::Kind::START, std::chrono::milliseconds(1), 2));

    REQUIRE_FALSE(child->isInitialized());
    REQUIRE_FALSE(parent->isInitialized());

    child->mockInit(a1);
    parent->initObject(a2);

    REQUIRE(child->isInitialized());
    REQUIRE(parent->isInitialized());

    auto self = child->testCreateSelfAction(std::chrono::milliseconds(2), std::chrono::milliseconds(1));
    REQUIRE(self->getAffected()->front() == child->getObjectId());
    REQUIRE(self->getKind() == Action::Kind::START);
    REQUIRE(self->getStartTime() == std::chrono::milliseconds(1));
    REQUIRE(self->getDuration() == std::chrono::milliseconds(2));
    REQUIRE(self->getActionData() == nullptr);
}

TEST_CASE( "BaseObject Test: Execution", "[BaseObject]" )
{
    {
        MockCore* c = new MockCore();
        setCoreP(c);
        std::shared_ptr<MockDebugObjectChild> child(new MockDebugObjectChild());
        auto mockClock = dynamic_cast<MockClock*>(c->getClock().get());
        auto mockActionManager = dynamic_cast<MockObjectActionManager*>(child->getMockedObjectManager());

        std::shared_ptr<Action> a1(new Action(std::chrono::milliseconds(1), Action::Kind::START, std::chrono::milliseconds(500), 1));
        std::shared_ptr<Action> a2(new Action(std::chrono::milliseconds(1), Action::Kind::END, std::chrono::milliseconds(1), 2));
        std::shared_ptr<Action> a3(new Action(std::chrono::milliseconds(1), Action::Kind::INIT, std::chrono::milliseconds(1), 2));


        REQUIRE_CALL(*mockActionManager, startOrDelay(a1))
                    .RETURN(false);

        REQUIRE_CALL(*c, scheduleAction(trompeloeil::_))
                    .WITH(std::shared_ptr<Action>(_1)->getKind() == Action::Kind::END)
                    .WITH(std::shared_ptr<Action>(_1)->getStartTime() == (a1->getStartTime() + a1->getDuration()));
        REQUIRE_CALL(*child, startExecution(a1));

        child->mockInit(a1);
        child->execute(a1);

        REQUIRE_CALL(*child, endExecution(a2));
        ALLOW_CALL(*mockClock, getSimTimeNow())
                    .RETURN(a1->getStartTime());
        child->execute(a2);

        REQUIRE_CALL(*child, initObject(a3));
        child->execute(a3);
    }

}
