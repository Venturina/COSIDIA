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

} // namespace cosidia