#include "core/Action.hpp"
#include "objects/BaseObject.hpp"
#include "utils/enforce.hpp"

#include <loguru/loguru.hpp>

namespace cosidia
{

const Symbol Action::default_type = "default"_sym;

Action::Action(Duration duration, TimePoint start, ObjectId obj, ObjectId generator) : mDuration(duration), mStartTime(start), mAffectedObject(obj), mGeneratingObject(generator)
{
    enforce(mAffectedObject.valid(), "Action: tried to add invalid object id to action");
    enforce(start >= TimePoint { Duration::zero() }, "Action: negative start time");
}

void Action::setActionData(std::shared_ptr<const ActionData> data)
{
    assert(mActionData == nullptr);
    mActionData = std::move(data);
}

void Action::shiftStartTime(TimePoint t)
{
    enforce(mStartTime <= t, "Action: new time point must be later the old time point");
    mStartTime = t;
}

} // ns cosidia