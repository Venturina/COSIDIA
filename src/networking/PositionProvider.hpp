#pragma once

#include "vanetza/common/position_provider.hpp"
#include "objects/VehicleObject.hpp"

namespace cosidia
{

class PositionProvider : public vanetza::PositionProvider
{
public:
    virtual const vanetza::PositionFix& position_fix() { return mPositionFix; };

    virtual void updatePosition(const VehicleObjectContext&);

private:
    // todo: avoid updating to often
    vanetza::PositionFix mPositionFix;
};

}
