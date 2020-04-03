
#include <catch2/catch.hpp>
#include "debug/DebugObject.hpp"
#include "core/Action.hpp"
#include "objects/ObjectList.hpp"

using namespace paresis;

TEST_CASE( "ObjectContainer", "[ObjectContainer]" ) {

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