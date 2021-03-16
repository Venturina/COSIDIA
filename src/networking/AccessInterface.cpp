#include "networking/AccessInterface.hpp"
#include "loguru/loguru.hpp"
#include "utils/enforce.hpp"

namespace cosidia
{

void AccessInterface::request(const vanetza::access::DataRequest& request, std::unique_ptr<vanetza::ChunkPacket> packet)
{
    enforce(!hasTransmissionRequest(), "AccessInterface: double transmission")
    DLOG_F(ERROR, "Transmission available");
    mCurrentTransmission = std::make_shared<Transmission>(request, std::move(packet));
    mHasRequest = true;
}

std::shared_ptr<Transmission> AccessInterface::getTransmission(std::shared_ptr<const ObjectContainer> container)
{
    enforce(hasTransmissionRequest(), "AccessInterface: transmission request without pending transmission");
    enforce(mCacheQuery, "AccessInterface: ObjectCache was not initialized");
    mHasRequest = false;
    DLOG_F(ERROR, "Transmission Taken");
    auto receivers = mObjectCache->getObjectsByType(container, mRequestObjectType, mCacheQuery.get());
    for(const auto& receiver : receivers) {
        auto base = getSiblingByName(receiver, "VehicleObject", container);
        auto mob = std::static_pointer_cast<VehicleObject>(base.lock());
        mCurrentTransmission->addPossibleReceiver(receiver, mob);
    }

    return std::move(mCurrentTransmission);
}

void AccessInterface::initializeCache(std::string typeIdentifier, ObjectId excludeId)
{
    mRequestObjectType = typeIdentifier;
    mObjectCache.reset(new SimpleObjectCache());
    mCacheQuery.reset(new CacheQueryExclude(excludeId));
}

void AccessInterface::dropTransmission()
{
    DLOG_F(ERROR, "Transmission dropped");
    mHasRequest = false;
}

} // namespace cosidia
