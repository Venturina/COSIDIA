#include "networking/ParesisAccessInterface.hpp"
#include "loguru/loguru.hpp"

namespace paresis
{

void ParesisAccessInterface::request(const vanetza::access::DataRequest& request, std::unique_ptr<vanetza::ChunkPacket> packet) {
    DLOG_F(ERROR, "Message from Vanetza");
    mInterfaceActionData = std::make_shared<AccesssInterfaceActionData>(request, std::move(packet));
}

} // namespace paresis
