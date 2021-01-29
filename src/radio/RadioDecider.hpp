#pragma once

#include "radio/fwd.hpp"
#include "radio/InterferenceDecider.hpp"
#include "radio/PhysicalDecider.hpp"

namespace cosidia
{
class RadioDecider
{
public:
    RadioDecider(std::unique_ptr<InterferenceDecider> i, std::unique_ptr<PhysicalDecider> p) :
        mInterferenceDecider(std::move(i)), mPhysicalDecider(std::move(p)) {}

    virtual std::list<ObjectId> decideOnPhysicalReception(const Transmission* t)
    {
        return mPhysicalDecider->decideOnPhysical(t);
    }

    virtual std::list<ObjectId> decideOnInterference(std::shared_ptr<const Transmission> t, const RadioContext* map)
    {
        return mInterferenceDecider->decideOnInterferences(t, map);
    }

private:
    std::unique_ptr<InterferenceDecider> mInterferenceDecider;
    std::unique_ptr<PhysicalDecider> mPhysicalDecider;
};

} // namespace cosidia