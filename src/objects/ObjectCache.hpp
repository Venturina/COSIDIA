#pragma once

#include "objects/CacheQuery.hpp"
#include "objects/ObjectList.hpp"

#include <functional>
#include <list>
#include <map>

namespace paresis
{

/**
 * Used to performantely search the ObjectContainer
 */
class ObjectCache
{
public:
    ObjectCache() = default;
    ~ObjectCache() = default;

    /**
     * Get list of object IDs which match the type string and the query
     * @param ObjectContainer to search
     * @param String to match the object type. Empty string matches all types
     * @param SearchQuery to restrict the search further. When CacheQuery::applyQuery evaluates to true, Object will be added to result list
     * @return List of Object IDs which match type and query
     */
    virtual std::list<ObjectId> getObjectsByType(std::shared_ptr<const ObjectContainer>, const std::string&, const CacheQuery* = nullptr) = 0;
};


/**
 * Naive Object cache which caches the requests as long as the ObjectContainer does not change.
 * When ObjectContainer changes, cache is rebuilt.
 */
class SimpleObjectCache : public ObjectCache
{
public:
    virtual std::list<ObjectId> getObjectsByType(std::shared_ptr<const ObjectContainer>, const std::string&, const CacheQuery* = nullptr) override;

protected:
    std::list<ObjectId> performSearch(const std::string&, const CacheQuery*);
    void resetQueries();

    std::weak_ptr<const ObjectContainer> mLastObjectContainer;
    std::map<std::pair<const std::string, const CacheQuery*>, std::list<ObjectId>> mCachedRequests;
};



} // namespace paresis
