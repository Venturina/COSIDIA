#ifndef TRANSMISSION_HPP_LNSFO
#define TRANSMISSION_HPP_LNSFO

#include <boost/geometry/geometries/point_xy.hpp>
#include "boost/date_time/posix_time/posix_time_types.hpp"

typedef boost::geometry::model::d2::point_xy<double> CartesianPoint;

class Transmission {
public:
    const CartesianPoint& getEmitterPosition();
    const boost::posix_time::ptime emittingTime;

private:
    CartesianPoint mEmitterPosition;
    boost::posix_time::ptime mEmittingTime;

};

#endif /* TRANSMISSION_HPP_LNSFO */