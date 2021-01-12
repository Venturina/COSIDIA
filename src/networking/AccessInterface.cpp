#include "networking/AccessInterface.hpp"
#include "loguru/loguru.hpp"
#include "utils/enforce.hpp"

namespace cosidia
{

void AccessInterface::request(const vanetza::access::DataRequest& request, std::unique_ptr<vanetza::ChunkPacket> packet)
{
    enforce(!hasTransmissionRequest(), "ParsisActionInterface: double transmission")
    DLOG_F(ERROR, "Message from Vanetza");
    mInterfaceActionData = std::make_shared<AccesssInterfaceActionData>(request, std::move(packet));
    mHasRequest = true;
}

std::pair<const std::list<ObjectId>, std::shared_ptr<const AccesssInterfaceActionData>> AccessInterface::getTransmission(std::shared_ptr<const ObjectContainer> container)
{
    enforce(hasTransmissionRequest(), "COSIDIAActionInterface: transmission request without pending transmission");
    enforce(mCacheQuery, "AccessInterface: ObjectCache was not initialized");
    mHasRequest = false;
    DLOG_F(ERROR, "Message taken");
    return std::make_pair(mObjectCache->getObjectsByType(container, mRequestObjectType, mCacheQuery.get()), getCurrentTransmission());
}

void AccessInterface::initializeCache(std::string typeIdentifier, ObjectId excludeId)
{
    mRequestObjectType = typeIdentifier;
    mObjectCache.reset(new SimpleObjectCache());
    mCacheQuery.reset(new CacheQueryExclude(excludeId));
}

void AccessInterface::dropTransmission()
{
    DLOG_F(ERROR, "Message dropped");
    mHasRequest = false;
}

} // namespace cosidia
