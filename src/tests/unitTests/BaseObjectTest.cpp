#include <catch2/catch.hpp>
#include <trompeloeil/include/catch2/trompeloeil.hpp>

#include "debug/DebugObject.hpp"
#include "debug/DebugAction.hpp"
#include "core/Action.hpp"
#include "core/ActionHandler.hpp"
#include "core/Core.hpp"
#include "core/InitAction.hpp"
#include "objects/ObjectActionManager.hpp"



using namespace cosidia;

class MockObjectActionManager : public ObjectActionManager
{
public:
    MockObjectActionManager(ObjectId& id) : ObjectActionManager(id) {}
    MAKE_MOCK1(startOrDelay, bool(std::shared_ptr<Action>), override);
};

class MockClock : public SteadyClock
{
public:
    MockClock() : SteadyClock(1) {};
    MAKE_CONST_MOCK0(getSimTimeNow, SimClock::time_point(), override);
};

class MockCore : public Core
{
public:
    MockCore() : Core() {
        mClock.reset(new MockClock());
    };

    MAKE_MOCK1(scheduleAction, void(HandlerP), override);
};

class MockDebugObjectChild : public DebugObjectChild
{
public:
    MockDebugObjectChild() : DebugObjectChild() {
        ObjectId id{1};
        mActionManager.reset(new MockObjectActionManager(id));
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

    //std::shared_ptr<Action> a1(new Action(std::chrono::milliseconds(1), Action::Kind::START, SimClock::atMillisecond(500), ObjectId { 1 }, ObjectId { 5 }));
    //std::shared_ptr<Action> a2(new Action(std::chrono::milliseconds(1), Action::Kind::START, SimClock::atMillisecond(1), ObjectId { 2 }, ObjectId { 5 }));

    std::shared_ptr<debug::DebugDurationAction> a1 = ActionFactory<debug::DebugDurationAction>::create(std::chrono::milliseconds(1), SimClock::atMillisecond(500), ObjectId { 1 }, ObjectId { 5 });
    std::shared_ptr<debug::DebugDurationAction> a2 = ActionFactory<debug::DebugDurationAction>::create(std::chrono::milliseconds(1), SimClock::atMillisecond(1), ObjectId { 2 }, ObjectId { 5 });


    SECTION("Create self Action") {
        REQUIRE_FALSE(child->isInitialized());
        REQUIRE_FALSE(parent->isInitialized());

        child->mockInit(a1);
        parent->initObject(a2);

        REQUIRE(child->isInitialized());
        REQUIRE(parent->isInitialized());

        auto self = child->testCreateSelfAction(std::chrono::milliseconds(2), SimClock::atMillisecond(1));
        REQUIRE(self->getAffected()== child->getObjectId());
        REQUIRE(self->getStartTime() == SimClock::atMillisecond(1));
        REQUIRE(self->getDuration() == std::chrono::milliseconds(2));
        REQUIRE(self->getActionData() == nullptr);
    }

    #ifdef COSIDIA_SAFE
    SECTION("Is Initialized") {
            REQUIRE_THROWS(parent->startExecutionBase(a1));
            REQUIRE_THROWS(a1->getStartHandler()->invoke(parent.get()));
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

        std::shared_ptr<debug::DebugDurationAction> a1 = ActionFactory<debug::DebugDurationAction>::create(std::chrono::milliseconds(1), SimClock::atMillisecond(500), ObjectId { 1 }, ObjectId { 5 });
        a1->setActionId(5);
        std::shared_ptr<debug::DebugInitAction> a3 = ActionFactory<debug::DebugInitAction>::create(std::chrono::milliseconds(1), SimClock::atMillisecond(1), ObjectId { 2 }, ObjectId { 5 });


        REQUIRE_CALL(*mockActionManager, startOrDelay(a1))      // call_1
                    .RETURN(false);

        /** just for refference
        REQUIRE_CALL(*c, scheduleAction(trompeloeil::_))        // call_2
                    .WITH(std::shared_ptr<Action>(_1)->getKind() == Action::Kind::END)
                    .WITH(std::shared_ptr<Action>(_1)->getStartTime() == (a1->getStartTime() + a1->getDuration()));

        **/
        REQUIRE_CALL(*c, scheduleAction(a1->getEndHandler()));
        REQUIRE_CALL(*child, startExecution(a1));               // call_2


        child->mockInit();                                      // initializes object properly
        a1->getStartHandler()->invoke(child.get());

        REQUIRE_CALL(*child, endExecution(a1));
        ALLOW_CALL(*mockClock, getSimTimeNow())
                    .RETURN(a1->getStartTime());
        a1->getEndHandler()->invoke(child.get());

        REQUIRE_CALL(*child, initObject(a3));
        a3->getInitHandler()->invoke(child.get());
    }
}