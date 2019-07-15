#include "core/Action.hpp"
#include "objects/BaseObject.hpp"

namespace paresis
{

Action::Action(std::chrono::nanoseconds duration, Kind k, std::chrono::nanoseconds start, std::shared_ptr<BaseObject> obj) : mDuration(duration), mKind(k), mStartTime(start)
{
    mAffectedObjects.push_back(obj);
    //auto objP = std::make_shared<object::BaseObject>(obj);
    //mAffectedObjects.push_back(std::make_shared<object::BaseObject>(obj));
}
Action::Action(std::chrono::nanoseconds duration, Kind k, std::chrono::nanoseconds start, std::list<std::shared_ptr<BaseObject>> obj) : mDuration(duration), mKind(k), mStartTime(start)
{
    mAffectedObjects = obj;
    //auto objP = std::make_shared<object::BaseObject>(obj);
    //mAffectedObjects.push_back(std::make_shared<object::BaseObject>(obj));
}


}