#include "networking/ParesisAccessInterface.hpp"
#include "loguru/loguru.hpp"
#include "utils/enforce.hpp"

namespace paresis
{

void ParesisAccessInterface::request(const vanetza::access::DataRequest& request, std::unique_ptr<vanetza::ChunkPacket> packet)
{
    enforce(!hasTransmissionRequest(), "ParsisActionInterface: double transmission")
    DLOG_F(ERROR, "Message from Vanetza");
    mInterfaceActionData = std::make_shared<AccesssInterfaceActionData>(request, std::move(packet));
    mHasRequest = true;
}

std::pair<const std::list<ObjectId>, std::shared_ptr<const AccesssInterfaceActionData>> ParesisAccessInterface::getTransmission(std::shared_ptr<const ObjectContainer> container)
{
    enforce(hasTransmissionRequest(), "ParesisActionInterface: transmission request without pending transmission");
    enforce(mCacheQuery, "ParesisAccessInterface: ObjectCache was not initialized");
    mHasRequest = false;
    DLOG_F(ERROR, "Message taken");
    return std::make_pair(mObjectCache->getObjectsByType(container, mRequestObjectType, mCacheQuery.get()), getCurrentTransmission());
}

void ParesisAccessInterface::initializeCache(std::string typeIdentifier, ObjectId excludeId)
{
    mRequestObjectType = typeIdentifier;
    mObjectCache.reset(new SimpleObjectCache());
    mCacheQuery.reset(new CacheQueryExclude(excludeId));
}

void ParesisAccessInterface::dropTransmission()
{
    DLOG_F(ERROR, "Message dropped");
    mHasRequest = false;
}

} // namespace paresis
