#include "core/Action.hpp"
#include "objects/BaseObject.hpp"
#include "utils/enforce.hpp"

#include <loguru/loguru.hpp>

namespace paresis
{

const Symbol Action::default_type = "default"_sym;

Action::Action(std::chrono::nanoseconds duration, Kind k, std::chrono::nanoseconds start, ObjectId obj, ObjectId generator) : mDuration(duration), mKind(k), mStartTime(start), mGeneratingObject(generator)
{
    assert(obj.valid());
    if(obj.valid()) {
        addAffected(obj);
    }
    enforce(start >= std::chrono::nanoseconds(0), "Action: negative start time");
    //auto objP = std::make_shared<object::BaseObject>(obj);
    //mAffectedObjects.push_back(std::make_shared<object::BaseObject>(obj));
}
Action::Action(std::chrono::nanoseconds duration, Kind k, std::chrono::nanoseconds start, std::list<ObjectId> obj, ObjectId generator) : mDuration(duration), mKind(k), mStartTime(start), mGeneratingObject(generator)
{
    enforce(start >= std::chrono::nanoseconds(0), "Action: negative start time");
    assert(std::all_of(obj.begin(), obj.end(), [](ObjectId id) { return id.valid(); }));
    for(auto id : obj) {
        if(id.valid()) {
            addAffected(id);
        }
    }
    //auto objP = std::make_shared<object::BaseObject>(obj);
    //mAffectedObjects.push_back(std::make_shared<object::BaseObject>(obj));
}

void Action::addAffected(ObjectId id){
    assert(id.valid());
    mAffectedObjects.push_back(id);
}

void Action::setStartTime(SteadyClock::duration start)
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

std::shared_ptr<Action> makeEndAction(std::shared_ptr<Action> beginAction, std::list<ObjectId> affected)
{
    enforce(beginAction->getActionId() != 0, "Action: ActionId is 0");
    enforce(beginAction->getBeginId() == 0, "Action: Tried to make endAction with already set beginActionId");
    if(beginAction->getActionData() == nullptr) {
        auto action = std::make_shared<Action>(std::chrono::nanoseconds{0}, Action::Kind::END, beginAction->getStartTime() + beginAction->getDuration(), affected, beginAction->getGeneratingObject());
        action->setType(beginAction->getType());
        action->setBeginId(beginAction->getActionId());
        return action;
    } else {
        auto a = std::make_shared<Action>(std::chrono::nanoseconds{0}, Action::Kind::END, beginAction->getStartTime() + beginAction->getDuration(), affected, beginAction->getGeneratingObject());
        a->setActionData(beginAction->getActionData());
        a->setType(beginAction->getType());
        a->setBeginId(beginAction->getActionId());
        return std::move(a);
    }
}


}