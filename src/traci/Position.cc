#include "traci/Position.h"

namespace traci
{

cosidia::Position position_cast(const Boundary& boundary, const TraCIPosition& pos)
{
    const double x = pos.x - boundary.lowerLeftPosition().x;
    const double y = boundary.upperRightPosition().y - pos.y;
    return cosidia::Position(x, y);
}

TraCIPosition position_cast(const Boundary& boundary, const cosidia::Position& pos)
{
    const double x = pos.x + boundary.lowerLeftPosition().x;
    const double y = boundary.upperRightPosition().y - pos.y;
    TraCIPosition tmp;
    tmp.x = x;
    tmp.y = y;
    tmp.z = 0.0;
    return tmp;
}

} // namespace traci
