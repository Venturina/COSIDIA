#include "networking/ParesisPositionProvider.hpp"

namespace paresis
{

void ParesisPositionProvider::updatePosition()
{

/*
    using namespace vanetza::units;
    static const TrueNorth north;

    auto geopos = mVehicleController->getGeoPosition();
    mPositionFix.timestamp = mRuntime->now();
    mPositionFix.latitude = geopos.latitude;
    mPositionFix.longitude = geopos.longitude;
    mPositionFix.confidence.semi_minor = 5.0 * si::meter;
    mPositionFix.confidence.semi_major = 5.0 * si::meter;
    mPositionFix.course.assign(north + GeoAngle { mVehicleController->getHeading().getTrueNorth() }, north + 3.0 * degree);
    mPositionFix.speed.assign(mVehicleController->getSpeed(), 1.0 * si::meter_per_second);

    // prevent signal listeners to modify our position data
    PositionFixObject tmp { mPositionFix };
    emit(scPositionFixSignal, &tmp);
*/

    using namespace vanetza::units;
    


}



}