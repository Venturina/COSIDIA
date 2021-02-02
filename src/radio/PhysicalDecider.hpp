#pragma once

#include "objects/ObjectId.hpp"
#include "radio/Transmission.hpp"
#include <random>

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


class LogDistancePhysicalDecider : public PhysicalDecider
{
public:
    LogDistancePhysicalDecider(double pathLossExponent = 2.0, double sigma = 0.0);

    std::list<ObjectId> decideOnPhysical(const Transmission*) override;

private:
    std::mt19937 mRandomGen;
    double mPathLossExponent;
    std::normal_distribution<double> mNormalNoise;
};

} // namespace cosidia