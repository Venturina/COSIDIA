#include "networking/PositionProvider.hpp"
#include "networking/Runtime.hpp"
#include "networking/VanetzaDefs.hpp"

namespace cosidia
{

void PositionProvider::updatePosition(const VehicleObjectContext& context)
{
    using namespace vanetza::units;
    static const TrueNorth north;

    mPositionFix.timestamp = Runtime::convert(context.lastUpdate);

    mPositionFix.confidence.semi_minor = 5.0 * si::meter;
    mPositionFix.confidence.semi_major = 5.0 * si::meter;

    mPositionFix.longitude = context.longitude * degrees;
    mPositionFix.latitude = context.latitude * degrees;

    mPositionFix.course.assign(context.heading * north, north + 3.0 * degree);
    mPositionFix.speed.assign(context.speed * si::meter_per_second, 1.0 * si::meter_per_second);
}

}