#ifndef PARESIS_POSITION_PROVIDER_SODNF
#define PARESIS_POSITION_PROVIDER_SODNF

#include "vanetza/common/position_provider.hpp"
#include "objects/VehicleObject.hpp"

namespace paresis
{

class ParesisPositionProvider : public vanetza::PositionProvider
{
public:
    virtual const vanetza::PositionFix& position_fix() { return mPositionFix; };

    virtual void updatePosition(const VehicleObjectContext&);

private:
    // todo: avoid updating to often
    vanetza::PositionFix mPositionFix;
};

}

#endif /* PARESIS_POSITION_PROVIDER_SODNF */