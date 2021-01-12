#include "objects/CacheQuery.hpp"

namespace cosidia
{

bool CacheQueryExclude::applyQuery(const BaseObject* element) const
{
    if(element->getObjectId() == mExcludeId) {
        return false;
    } else {
        return true;
    }
}

} // namespace cosidia