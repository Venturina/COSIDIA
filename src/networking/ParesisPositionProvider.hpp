#ifndef PARESIS_POSITION_PROVIDER_SODNF
#define PARESIS_POSITION_PROVIDER_SODNF

#include "vanetza/common/position_provider.hpp"

class ParesisPositionProvider : public vanetza::PositionProvider
{
public:
    virtual const vanetza::PositionFix& position_fix() {};

    virtual void updatePosition();

private:
    // todo: avoid updating to often
    vanetza::PositionFix mPositionFix;
};

#endif /* PARESIS_POSITION_PROVIDER_SODNF */