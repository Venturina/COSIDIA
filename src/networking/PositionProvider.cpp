#include "networking/PositionProvider.hpp"
#include "networking/VanetzaDefs.hpp"

namespace cosidia
{



void PositionProvider::updatePosition(const VehicleObjectContext& context)
{
    using namespace vanetza::units;
    static const TrueNorth north;

    auto timeSincePosix = context.lastUpdate.time_since_epoch();
    vanetza::Clock::time_point ts { std::chrono::duration_cast<vanetza::Clock::duration>(timeSincePosix - utcItsDiff) };
    mPositionFix.timestamp = ts;

    mPositionFix.confidence.semi_minor = 5.0 * si::meter;
    mPositionFix.confidence.semi_major = 5.0 * si::meter;

    mPositionFix.longitude = context.longitude * degrees;
    mPositionFix.latitude = context.latitude * degrees;

    mPositionFix.course.assign(context.heading * north, north + 3.0 * degree);
    mPositionFix.speed.assign(context.speed * si::meter_per_second, 1.0 * si::meter_per_second);
}

}