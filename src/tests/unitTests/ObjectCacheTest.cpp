#include <catch2/catch.hpp>
#include <trompeloeil/include/catch2/trompeloeil.hpp>

#include "core/ActionList.hpp"
#include "core/Core.hpp"
#include "objects/CacheQuery.hpp"
#include "objects/ObjectCache.hpp"
#include "debug/DebugObject.hpp"


using namespace paresis;

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

class TestObjectCache : public SimpleObjectCache
{
public:
    TestObjectCache() = default;
    int sampleSize() { return mCachedRequests.size(); }
};

TEST_CASE( "SimpleObjectCache", "[SimpleObjectCache]" ) {
MockCore c;

std::shared_ptr<ObjectContainer> container1(new ObjectContainer());
std::shared_ptr<ObjectContainer> container2(new ObjectContainer());

TestObjectCache cache;

std::shared_ptr<DebugObject> obj1(new DebugObject());
obj1->setObjectId(1);
obj1->setName("foo");
obj1->addParent(0);
std::shared_ptr<DebugObject> obj2(new DebugObject());
obj2->setObjectId(2);
obj2->setName("bar");
obj2->addParent(0);
std::shared_ptr<DebugObject> obj3(new DebugObject());
obj3->setObjectId(3);
obj3->setName("foo");
obj3->addParent(0);
std::shared_ptr<DebugObject> obj4(new DebugObject());
obj4->setObjectId(4);
obj4->setName("foo");
obj4->addParent(0);
std::shared_ptr<DebugObject> obj5(new DebugObject());
obj5->setObjectId(5);
obj5->setName("bar");
obj5->addParent(0);
std::shared_ptr<DebugObject> obj6(new DebugObject());
obj6->setObjectId(6);
obj6->setName("foo");
obj6->addParent(0);

container1->insert(obj1);
container1->insert(obj2);
container1->insert(obj3);
container1->insert(obj4);

container2->insert(obj1);
container2->insert(obj2);
container2->insert(obj3);
container2->insert(obj4);
container2->insert(obj5);
container2->insert(obj6);



auto list = cache.getObjectsByType(container1, "foo");
REQUIRE(list.size() == 3);
REQUIRE(cache.sampleSize() == 1);

list = cache.getObjectsByType(container1, "foo");
REQUIRE(list.size() == 3);
REQUIRE(cache.sampleSize() == 1);

list = cache.getObjectsByType(container1, "bar");
REQUIRE(list.size() == 1);
REQUIRE(cache.sampleSize() == 2);

list = cache.getObjectsByType(container1, "bar");
REQUIRE(list.size() == 1);
REQUIRE(cache.sampleSize() == 2);

list = cache.getObjectsByType(container2, "bar");
REQUIRE(list.size() == 2);
REQUIRE(cache.sampleSize() == 1);

list = cache.getObjectsByType(container1, "foo");
REQUIRE(list.size() == 3);
REQUIRE(cache.sampleSize() == 1);

list = cache.getObjectsByType(container1, "foo");
REQUIRE(list.size() == 3);
REQUIRE(cache.sampleSize() == 1);

list = cache.getObjectsByType(container2, "bar");
REQUIRE(list.size() == 2);
REQUIRE(cache.sampleSize() == 1);

list = cache.getObjectsByType(container2, "foo");
REQUIRE(list.size() == 4);
REQUIRE(cache.sampleSize() == 2);

list = cache.getObjectsByType(container2, "baz");
REQUIRE(list.size() == 0);
REQUIRE(cache.sampleSize() == 3);

list = cache.getObjectsByType(container1, "baz");
REQUIRE(list.size() == 0);
REQUIRE(cache.sampleSize() == 1);

list = cache.getObjectsByType(container2, "");
REQUIRE(list.size() == 6);
REQUIRE(cache.sampleSize() == 1);

}



TEST_CASE( "SimpleObjectCacheWithQuery", "[SimpleObjectCache]" ) {
MockCore c;

std::shared_ptr<ObjectContainer> container1(new ObjectContainer());
std::shared_ptr<ObjectContainer> container2(new ObjectContainer());
std::unique_ptr<CacheQuery> query(new CacheQueryExclude(3));
std::unique_ptr<CacheQuery> query2(new CacheQueryExclude(4));

TestObjectCache cache;

std::shared_ptr<DebugObject> obj1(new DebugObject());
obj1->setObjectId(1);
obj1->setName("foo");
obj1->addParent(0);
std::shared_ptr<DebugObject> obj2(new DebugObject());
obj2->setObjectId(2);
obj2->setName("bar");
obj2->addParent(0);
std::shared_ptr<DebugObject> obj3(new DebugObject());
obj3->setObjectId(3);
obj3->setName("foo");
obj3->addParent(0);
std::shared_ptr<DebugObject> obj4(new DebugObject());
obj4->setObjectId(4);
obj4->setName("foo");
obj4->addParent(0);
std::shared_ptr<DebugObject> obj5(new DebugObject());
obj5->setObjectId(5);
obj5->setName("bar");
obj5->addParent(0);
std::shared_ptr<DebugObject> obj6(new DebugObject());
obj6->setObjectId(6);
obj6->setName("foo");
obj6->addParent(0);

container1->insert(obj1);
container1->insert(obj2);
container1->insert(obj3);
container1->insert(obj4);

container2->insert(obj1);
container2->insert(obj2);
container2->insert(obj3);
container2->insert(obj4);
container2->insert(obj5);
container2->insert(obj6);


auto list = cache.getObjectsByType(container1, "foo");
REQUIRE(list.size() == 3);
REQUIRE(cache.sampleSize() == 1);

list = cache.getObjectsByType(container1, "foo", query.get());
REQUIRE(list.size() == 2);
REQUIRE(cache.sampleSize() == 2);

list = cache.getObjectsByType(container2, "foo");
REQUIRE(list.size() == 4);
REQUIRE(cache.sampleSize() == 1);

list = cache.getObjectsByType(container2, "foo", query.get());
REQUIRE(list.size() == 3);
REQUIRE(cache.sampleSize() == 2);

list = cache.getObjectsByType(container2, "bar", query.get());
REQUIRE(list.size() == 2);
REQUIRE(cache.sampleSize() == 3);

list = cache.getObjectsByType(container2, "bar", query2.get());
REQUIRE(list.size() == 2);
REQUIRE(cache.sampleSize() == 4);

list = cache.getObjectsByType(container2, "", query2.get());
REQUIRE(list.size() == 5);
REQUIRE(cache.sampleSize() == 5);

list = cache.getObjectsByType(container2, "", query.get());
REQUIRE(list.size() == 5);
REQUIRE(cache.sampleSize() == 6);

list = cache.getObjectsByType(container2, "");
REQUIRE(list.size() == 6);
REQUIRE(cache.sampleSize() == 7);

list = cache.getObjectsByType(container1, "");
REQUIRE(list.size() == 4);
REQUIRE(cache.sampleSize() == 1);

}