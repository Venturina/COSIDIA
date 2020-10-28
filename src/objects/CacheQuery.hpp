#pragma once
#include "objects/BaseObject.hpp"

namespace paresis
{

/**
 * CacheQuery is used to restrict the search in the ObjectCache
 */
class CacheQuery
{
public:
    CacheQuery() = default;
    ~CacheQuery() = default;

    /**
     * Method is applied on each Object in the ObjectCache
     * @param Object against which the Query should be tested
     * @return true if Object should be added to result list
     */
    virtual bool applyQuery(const BaseObject*) const = 0;
};


/**
 * Simple Query which excludes a specific ObjectId from result list in ObjectCache
 */
class CacheQueryExclude : public CacheQuery
{
public:
    CacheQueryExclude(int excludeId) : mExcludeId(excludeId), CacheQuery() {};
    ~CacheQueryExclude() = default;

    virtual bool applyQuery(const BaseObject*) const override;
private:
    int mExcludeId = 0;
};


} // namespace paresis