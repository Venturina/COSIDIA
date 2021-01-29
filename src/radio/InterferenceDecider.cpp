#include "radio/InterferenceDecider.hpp"

namespace cosidia
{

std::list<ObjectId> NoInterferenceDecider::decideOnInterferences(std::shared_ptr<const Transmission> transmission, const RadioContext* context)
{
    auto element = context->find(transmission);
    return element->second;
}

} // namespace cosidia
