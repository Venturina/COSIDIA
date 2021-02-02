#ifndef GEOMETRY_H_W8CYK9GM
#define GEOMETRY_H_W8CYK9GM

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/geometries.hpp>
#include <boost/geometry/geometries/register/point.hpp>
#include <boost/geometry/geometries/register/ring.hpp>
#include <boost/math/constants/constants.hpp>
#include <vector>

namespace cosidia
{
struct Position
{
    using value_type = double;

    explicit Position(double px, double py) : x(px), y(py) {}
    Position() = default;
    Position(const Position&) = default;
    Position& operator=(const Position&) = default;

    value_type x = 0.0;
    value_type y = 0.0;
};

bool operator==(const Position&, const Position&);
bool operator!=(const Position&, const Position&);
Position::value_type distance(const Position&, const Position&);

struct GeoPosition
{
    using value_type = double;

    GeoPosition() = default;
    GeoPosition(const GeoPosition&) = default;
    GeoPosition& operator=(const GeoPosition&) = default;

    value_type latitude;
    value_type longitude;
};

struct Angle
{
    using value_type = double;
    static constexpr double pi = boost::math::double_constants::pi;

    Angle() = default;
    Angle(const Angle&) = default;
    Angle& operator=(const Angle&) = default;

    explicit constexpr Angle(value_type angle) : value(angle) {}
    static constexpr Angle from_rad(double rad) { return Angle(rad); }
    static constexpr Angle from_deg(double deg) { return Angle(deg / 180.0 * pi); }

    constexpr double radian() const { return value; }
    constexpr double degree() const { return value * 180.0 / pi; }
    value_type getTrueNorth() const;

    value_type value;
};

} // namespace artery

// this register macro needs to be outside of any namespaces
BOOST_GEOMETRY_REGISTER_RING(std::vector<cosidia::Position>)

namespace boost { namespace geometry { namespace traits
{

BOOST_GEOMETRY_DETAIL_SPECIALIZE_POINT_TRAITS(cosidia::Position, 2, double, cs::cartesian)

template<> struct access<cosidia::Position, 0>
{
    static inline double get(const cosidia::Position& pos) { return pos.x; }
    static inline void set(cosidia::Position& pos, double v) { pos.x = v; }
};

template<> struct access<cosidia::Position, 1>
{
    static inline double get(const cosidia::Position& pos) { return pos.y; }
    static inline void set(cosidia::Position& pos, double v) { pos.y = v; }
};

template<> struct point_order<std::vector<cosidia::Position>>
{
    static const order_selector value = clockwise;
};

template<> struct closure<std::vector<cosidia::Position>>
{
    static const closure_selector value = open;
};

} // namespace traits
} // namespace geometry
} // namespace boost

#endif /* GEOMETRY_H_W8CYK9GM */

