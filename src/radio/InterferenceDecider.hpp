#pragma once

#include "objects/ObjectId.hpp"
#include "radio/fwd.hpp"
#include "radio/Transmission.hpp"

namespace cosidia
{

class InterferenceDecider
{
public:
    virtual std::list<ObjectId> decideOnInterferences(std::shared_ptr<const Transmission>, const RadioContext*) = 0;
};

class NoInterferenceDecider : public InterferenceDecider
{
public:
    std::list<ObjectId> decideOnInterferences(std::shared_ptr<const Transmission>, const RadioContext*);
};

} // namespace cosidia


