#pragma once
#include "objects/BaseObject.hpp"

namespace cosidia
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
    CacheQueryExclude(ObjectId excludeId) : mExcludeId(excludeId), CacheQuery() {};
    ~CacheQueryExclude() = default;

    virtual bool applyQuery(const BaseObject*) const override;
private:
    ObjectId mExcludeId;
};


} // namespace cosidia