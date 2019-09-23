#include "core/Action.hpp"
#include "objects/BaseObject.hpp"

#include <loguru/loguru.hpp>

namespace paresis
{

Action::Action(std::chrono::nanoseconds duration, Kind k, std::chrono::nanoseconds start, int obj) : mDuration(duration), mKind(k), mStartTime(start)
{
    assert(obj != -1);
    if(obj >= 0) {
        addAffected(obj);
    }
    //auto objP = std::make_shared<object::BaseObject>(obj);
    //mAffectedObjects.push_back(std::make_shared<object::BaseObject>(obj));
}
Action::Action(std::chrono::nanoseconds duration, Kind k, std::chrono::nanoseconds start, std::list<int> obj) : mDuration(duration), mKind(k), mStartTime(start)
{
    assert( !(std::find(obj.begin(), obj.end(), -1) != obj.end()));
    for(auto id : obj) {
        if(id >= 0) {
            addAffected(id);
        }
    }
    //auto objP = std::make_shared<object::BaseObject>(obj);
    //mAffectedObjects.push_back(std::make_shared<object::BaseObject>(obj));
}

void Action::addAffected(int id){
    assert(id >= 0);
    mAffectedObjects.push_back(id);
}

void Action::setStartTime(SteadyClock::duration start)
{
    mStartTime = start;
}


}