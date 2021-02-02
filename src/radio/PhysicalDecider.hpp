#pragma once

#include "objects/ObjectId.hpp"
#include "radio/Transmission.hpp"

namespace cosidia
{

class PhysicalDecider
{
public:
    virtual std::list<ObjectId> decideOnPhysical(const Transmission*) = 0;
};


class MessagesForEveryonePhysicalDecider : public PhysicalDecider
{
public:
    std::list<ObjectId> decideOnPhysical(const Transmission*) override;
};


//TODO: implement this!
class SimpleDistancePhysicalDecider : public PhysicalDecider
{
public:
    SimpleDistancePhysicalDecider(int distance) : mDistance(distance) {};

    std::list<ObjectId> decideOnPhysical(const Transmission*) override;

private:
    int mDistance;  // transmission distance in meters
};

} // namespace cosidia