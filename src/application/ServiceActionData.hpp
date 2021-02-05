#pragma once

#include "actionData/ActionData.hpp"

#include <vanetza/btp/data_request.hpp>
#include <vanetza/btp/header.hpp>
#include <vanetza/geonet/interface.hpp>
#include <vanetza/geonet/router.hpp>

namespace cosidia
{

class ServiceActionData : public ActionData
{
public:
    ServiceActionData() : ActionData("") {}

    vanetza::btp::DataRequestGeoNetParams request;
    std::shared_ptr<vanetza::DownPacket> packet;

    vanetza::btp::port_type port;
};

} // namespace cosidia
