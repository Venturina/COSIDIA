#include "objects/BaseObject.hpp"
#include "objects/ObjectCache.hpp"
#include "utils/enforce.hpp"

namespace paresis
{

std::list<int> SimpleObjectCache::getObjectsByType(std::shared_ptr<const ObjectContainer> container, const std::string& type, const CacheQuery* query)
{
    if(mLastObjectContainer.expired() || mLastObjectContainer.lock().get() != container.get()) {
        resetQueries();
        mLastObjectContainer = container;
        return performSearch(type, query);
    } else if(mLastObjectContainer.lock().get() == container.get()) {
        return performSearch(type, query);
    } else {
        enforce(false, "SimpleObjectCache: something went wrong")
    }
}

std::list<int> SimpleObjectCache::performSearch(const std::string& type, const CacheQuery* query)
{
    auto hit = mCachedRequests.find(std::make_pair(type, query));
    if (hit != mCachedRequests.end()) {
        return hit->second;
    } else {
        for(const auto& elem : mLastObjectContainer.lock()->getAll()) {
            if(elem.second->getObjectName() == type || type == "") {
                if(!query) {
                    mCachedRequests[std::make_pair(type, query)].push_back(elem.second->getObjectId());
                } else if(query->applyQuery(elem.second.get())) {
                    mCachedRequests[std::make_pair(type, query)].push_back(elem.second->getObjectId());
                }
            }
        }
        return mCachedRequests[std::make_pair(type, query)];
    }
}

void SimpleObjectCache::resetQueries()
{
    mCachedRequests.clear();
}

} // namespace paresis
