#pragma once

#include "networking/ParesisAccessInterface.hpp"
#include "vanetza/dcc/data_request.hpp"
#include "vanetza/dcc/mapping.hpp"
#include "vanetza/dcc/interface.hpp"

class DccPassThrough : public vanetza::dcc::RequestInterface
{
public:
    DccPassThrough(vanetza::access::Interface& access) : m_access(access) {}

    void request(const vanetza::dcc::DataRequest& dcc_req, std::unique_ptr<vanetza::ChunkPacket> packet) override
    {
        vanetza::access::DataRequest acc_req;
        acc_req.source_addr = dcc_req.source;
        acc_req.destination_addr = dcc_req.destination;
        acc_req.ether_type = dcc_req.ether_type;
        acc_req.access_category = vanetza::dcc::map_profile_onto_ac(dcc_req.dcc_profile);

        m_access.request(acc_req, std::move(packet));
    }

private:
    vanetza::access::Interface& m_access;
};