#include "core/Action.hpp"
#include "objects/BaseObject.hpp"

namespace paresis
{

Action::Action(uint32_t duration, Kind k, uint64_t start, std::shared_ptr<object::BaseObject> obj) : mDuration(duration), mKind(k), mStartTime(start)
{
    //auto objP = std::make_shared<object::BaseObject>(obj);
    //mAffectedObjects.push_back(std::make_shared<object::BaseObject>(obj));
}

}