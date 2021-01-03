#pragma once

#include "actionData/ParesisAccessInterfaceActionData.hpp"
#include "objects/CacheQuery.hpp"
#include "objects/ObjectCache.hpp"
#include "objects/ObjectList.hpp"
#include "vanetza/access/interface.hpp"

namespace paresis
{

class ParesisAccessInterface : public vanetza::access::Interface
{
public:
    ~ParesisAccessInterface() = default;

    virtual void request(const vanetza::access::DataRequest&, std::unique_ptr<vanetza::ChunkPacket>) override;

    bool hasTransmissionRequest() const { return mHasRequest; }
    std::pair<const std::list<ObjectId>, std::shared_ptr<const AccesssInterfaceActionData>> getTransmission(std::shared_ptr<const ObjectContainer>);

    void initializeCache(std::string, ObjectId);

    void dropTransmission();

private:
    bool mHasRequest = false;
    std::shared_ptr<AccesssInterfaceActionData> mInterfaceActionData;
    std::shared_ptr<AccesssInterfaceActionData> getCurrentTransmission() { return std::move(mInterfaceActionData); }

    std::unique_ptr<ObjectCache> mObjectCache;
    std::unique_ptr<CacheQuery> mCacheQuery;
    std::string mRequestObjectType = "none";
};


} // namespace paresis
