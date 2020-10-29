#pragma once

#include "actionData/ActionData.hpp"

#include "vanetza/access/data_request.hpp"
#include "vanetza/net/chunk_packet.hpp"

namespace paresis
{


class AccesssInterfaceActionData : public ActionData
{
public:
    AccesssInterfaceActionData(const vanetza::access::DataRequest request, std::unique_ptr<vanetza::ChunkPacket> packet):
            mRequest(request), mPacket(std::move(packet)), ActionData("AccessInterface") { }

private:
    const vanetza::access::DataRequest mRequest;
    std::shared_ptr<const vanetza::ChunkPacket> mPacket;
};


} // namespace paresis