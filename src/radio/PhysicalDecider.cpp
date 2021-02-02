#include "radio/PhysicalDecider.hpp"


namespace cosidia
{

std::list<ObjectId> MessagesForEveryonePhysicalDecider::decideOnPhysical(const Transmission* t)
{
    std::list<ObjectId> list;
    enforce(t->isSetup(), "MessagesForEveryonePhysicalDecider: transmission not setup!")

    for(const auto& receiver : t->getPossibleReceivers()) {
        list.push_back(receiver.first);
    }

    return list;
}

std::list<ObjectId> SimpleDistancePhysicalDecider::decideOnPhysical(const Transmission* t)
{
    std::list<ObjectId> list;
    enforce(t->isSetup(), "SimpleDistancePhysicalDecider: transmission is not set-up!")

    Position tx = t->getTransmitterPosition();

    for(const auto& receiver : t->getPossibleReceivers()) {
        const Position& rx = receiver.second.receiverContext->position;
        if (distance(tx, rx) <= mDistance) {
            list.push_back(receiver.first);
        }
    }

    return list;
}

LogDistancePhysicalDecider::LogDistancePhysicalDecider(double pathLossExponent, double sigma) :
    mPathLossExponent(pathLossExponent), mNormalNoise(0.0, sigma)
{
}

std::list<ObjectId> LogDistancePhysicalDecider::decideOnPhysical(const Transmission* t)
{
    std::list<ObjectId> list;
    enforce(t->isSetup(), "LogDistancePhysicalDecider: transmission is not set-up!")

    static constexpr double pi = boost::math::double_constants::pi;
    static constexpr double waveLength = 0.05081228101694915; // wavelength in meter for 5.9 GHz signal
    static const double pathLossRef = 20.0 * std::log10(4.0 * pi / waveLength);

    // TODO: rxThreshold should be a property of each receiver radio
    const double rxThreshold = -98.0; // weakest signal receivable by radio [dBm]

    const Position txPosition = t->getTransmitterPosition();
    const double txPower = t->getTransmissionPower();

    for(const auto& receiver : t->getPossibleReceivers()) {
        const Position& rxPosition = receiver.second.receiverContext->position;

        double dist = distance(txPosition, rxPosition);
        double pathLoss = 10.0 * mPathLossExponent * std::log10(dist) + mNormalNoise(mRandomGen);

        if (txPower - pathLoss > rxThreshold) {
            list.push_back(receiver.first);
        }
    }

    return list;
}

} // namespace cosidia