#include "utils/Geometry.h"
#include <boost/geometry/algorithms/distance.hpp>
#include <boost/math/constants/constants.hpp>
#include <cassert>
#include <cmath>

namespace cosidia
{

Angle::value_type Angle::getTrueNorth() const
{
    static const auto pi = boost::math::constants::pi<double>();

    value_type heading = value;
    // change rotation ccw -> cw
    heading *= -1.0;
    // rotate zero from east to north
    heading += 0.5 * pi;
    // normalize angle to [0; 2*pi[
    heading -= 2.0 * pi * std::floor(heading / (2.0 * pi));

    assert(heading >= 0.0);
    assert(heading < 2.0 * pi);
    return heading;
}

Position::value_type distance(const Position& a, const Position& b)
{
    return boost::geometry::distance(a, b);
}

bool operator==(const Position& a, const Position& b)
{
    return !(a != b);
}

bool operator!=(const Position& a, const Position& b)
{
    return a.x != b.x || a.y != b.y;
}

} // namespace artery
