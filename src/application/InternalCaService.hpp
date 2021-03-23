#pragma once

#include <vanetza/asn1/cam.hpp>
#include <vanetza/common/position_fix.hpp>
#include <core/Action.hpp>

#include "vanetza/geonet/router.hpp"
#include "vanetza/geonet/mib.hpp"

namespace cosidia
{

class Router;

class InternalCaService
{
public:
    InternalCaService(Router*);

    bool triggerCam(const vanetza::PositionFix*, Action*, vanetza::geonet::Router*, vanetza::geonet::MIB&);
    vanetza::asn1::Cam makeCam(const vanetza::PositionFix*, Action*);

private:
    /* data */
    Router* mRouter;

};

} // namespace cosidia

