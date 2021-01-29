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

} // namespace cosidia