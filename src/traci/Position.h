#ifndef POSITION_H_ECOTFZI7
#define POSITION_H_ECOTFZI7

#include "utils/Geometry.h"
#include "traci/Boundary.h"
#include "traci/sumo/utils/traci/TraCIAPI.h"

namespace traci
{

using libsumo::TraCIPosition;

cosidia::Position position_cast(const Boundary&, const TraCIPosition&);
TraCIPosition position_cast(const Boundary&, const cosidia::Position&);

} // namespace traci

#endif /* POSITION_H_ECOTFZI7 */

