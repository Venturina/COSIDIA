#include "core/Action.hpp"
#include "objects/BaseObject.hpp"
#include "utils/enforce.hpp"

#include <loguru/loguru.hpp>

namespace cosidia
{

const Symbol Action::default_type = "default"_sym;

Action::Action(Duration duration, Kind k, TimePoint start, ObjectId obj, ObjectId generator) : mDuration(duration), mKind(k), mStartTime(start), mAffectedObject(obj), mGeneratingObject(generator)
{
    enforce(mAffectedObject.valid(), "Action: tried to add invalid object id to action");
    enforce(start >= TimePoint { Duration::zero() }, "Action: negative start time");
}

void Action::setStartTime(TimePoint start)
{
    mStartTime = start;
}

void Action::setBeginId(int id)
{
    mBeginActionId = id;
}

void Action::setActionData(std::shared_ptr<const ActionData> data)
{
    assert(mActionData == nullptr);
    mActionData = std::move(data);
}

std::shared_ptr<Action> makeEndAction(std::shared_ptr<Action> beginAction)
{
    enforce(beginAction->getActionId() != 0, "Action: ActionId is 0");
    enforce(beginAction->getBeginId() == 0, "Action: Tried to make endAction with already set beginActionId");
    if(beginAction->getActionData() == nullptr) {
        auto endAction = std::make_shared<Action>(std::chrono::nanoseconds{0}, Action::Kind::END, beginAction->getStartTime() + beginAction->getDuration(), beginAction->getAffected(), beginAction->getGeneratingObject());
        endAction->setBeginId(beginAction->getActionId());
        endAction->setType(beginAction->getType());
        return endAction;
    } else {
        auto a = std::make_shared<Action>(std::chrono::nanoseconds{0}, Action::Kind::END, beginAction->getStartTime() + beginAction->getDuration(), beginAction->getAffected(), beginAction->getGeneratingObject());
        a->setActionData(beginAction->getActionData());
        a->setBeginId(beginAction->getActionId());
        a->setType(beginAction->getType());
        return a;
    }
}

} // ns cosidia