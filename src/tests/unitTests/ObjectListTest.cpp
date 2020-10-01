
#include <catch2/catch.hpp>
#include "core/Core.hpp"
#include "debug/DebugObject.hpp"
#include "core/Action.hpp"
#include "objects/ObjectList.hpp"

using namespace paresis;

TEST_CASE( "ObjectContainer", "[ObjectContainer]" ) {
    Core c;

    ObjectContainer_ptr o(new ObjectContainer);
    std::shared_ptr<DebugObjectChild> child(new DebugObjectChild);
    std::shared_ptr<DebugObjectParent> parent(new DebugObjectParent);


    #ifdef PARESIS_SAFE
    SECTION( "Test invariants" ) {
        REQUIRE_THROWS(o->insert(child));
        child->initObject();
        parent->initObject();
        REQUIRE_NOTHROW(o->insert(child));
        REQUIRE_THROWS(o->remove(parent));
        REQUIRE_THROWS(o->getObject(parent->getObjectId()));
    }

    SECTION( "Test unique invariants") {
        REQUIRE_THROWS(o->insertUnique(child));
        child->initObject();
        parent->initObject();
        o->insertUnique(child);
        REQUIRE_THROWS(o->insertUnique(child));
        REQUIRE_THROWS(o->getUnique(parent->getObjectName()));
        REQUIRE_NOTHROW(o->insert(parent));
        REQUIRE_THROWS(o->insertUnique(parent));
        REQUIRE_THROWS(o->getUnique(parent->getObjectName()));
    }

    SECTION("Test Remove")
    {
        child->initObject();
        o->insert(child);
        REQUIRE_NOTHROW(o->getObject(child->getObjectId()));
        REQUIRE_NOTHROW(o->remove(child->getObjectId()));
        REQUIRE_THROWS(o->getObject(child->getObjectId()));
    }
    #endif

    SECTION( "Test positive cases" ) {
        child->initObject();
        parent->initObject();
        o->insert(child);
        REQUIRE(o->getObject(child->getObjectId()) == child);
        o->insertUnique(parent);
        REQUIRE(o->getObject(parent->getObjectId()) == parent);
        REQUIRE(o->getUnique(parent->getObjectName()) == parent);
    }

}

TEST_CASE("ObjectList", "[ObjectList]") {
    ObjectList list;
    std::shared_ptr<DebugObjectChild> child(new DebugObjectChild);
    child->initObject();
    std::shared_ptr<DebugObjectParent> parent(new DebugObjectParent);
    parent->initObject();

    #ifdef PARESIS_SAFE
    SECTION("Insert") {
        auto cur1 = list.getCurrentObjectContainer();
        REQUIRE_THROWS(list.getObjectByIdFromCurrentContainer(child->getObjectId()));
        REQUIRE_THROWS(cur1->getObject(child->getObjectId()));
        list.addToObjectContainer(child->getObjectId(), child);
        REQUIRE_THROWS(cur1->getObject(child->getObjectId()));
        REQUIRE(list.getObjectByIdFromCurrentContainer(child->getObjectId()) == child);
        auto cur2 = list.getCurrentObjectContainer();
        REQUIRE(cur2->getObject(child->getObjectId()) == child);
        REQUIRE(list.getObjectByIdFromCurrentContainer(child->getObjectId()) == child);
    }

    SECTION("InsertUnique") {
        auto cur1 = list.getCurrentObjectContainer();
        REQUIRE_THROWS(list.getObjectByIdFromCurrentContainer(child->getObjectId()));
        REQUIRE_THROWS(cur1->getObject(child->getObjectId()));
        list.addUnique(child);
        REQUIRE_THROWS(cur1->getObject(child->getObjectId()));
        REQUIRE(list.getUniqueObjectByName(child->getObjectName()) == child);
        auto cur2 = list.getCurrentObjectContainer();
        REQUIRE(cur2->getObject(child->getObjectId()) == child);
        REQUIRE(list.getUniqueObjectByName(child->getObjectName()) == child);
    }
    #endif

}