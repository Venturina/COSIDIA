#pragma once

#include "actionData/AccessInterfaceActionData.hpp"
#include "objects/CacheQuery.hpp"
#include "objects/ObjectCache.hpp"
#include "objects/ObjectList.hpp"
#include "radio/Transmission.hpp"
#include "vanetza/access/interface.hpp"

namespace cosidia
{

class AccessInterface : public vanetza::access::Interface
{
public:
    ~AccessInterface() = default;

    virtual void request(const vanetza::access::DataRequest&, std::unique_ptr<vanetza::ChunkPacket>) override;

    bool hasTransmissionRequest() const { return mHasRequest; }
    std::shared_ptr<Transmission> getTransmission(std::shared_ptr<const ObjectContainer>);

    void initializeCache(std::string, ObjectId);

    void dropTransmission();

private:
    bool mHasRequest = false;
    std::shared_ptr<Transmission> mCurrentTransmission;
    std::shared_ptr<Transmission> getCurrentTransmission() { return std::move(mCurrentTransmission); }

    std::unique_ptr<ObjectCache> mObjectCache;
    std::unique_ptr<CacheQuery> mCacheQuery;
    std::string mRequestObjectType = "none";
};


} // namespace cosidia
