#ifndef _ACTION_HPP_NVOS
#define _ACTION_HPP_NVOS


#include <chrono>
#include <list>
#include <memory>
#include "actionData/ActionData.hpp"
#include "core/SteadyClock.hpp"
#include "objects/ObjectContext.hpp"


namespace paresis
{

class BaseObject;

typedef int ActionId;

/** A action describes
 *  a specific action in the paresis system.
 * It contains a start time point, a duration and something to execute
 */

class Action
{
public:
    enum class Kind {
        INIT,
        START,
        END
    };

    //Action(uint32_t duration) : mDuration(duration) {}
    Action(SteadyClock::duration duration, Kind k, SteadyClock::duration start, int id);
    Action(SteadyClock::duration duration, Kind k, SteadyClock::duration start, std::list<int> ids);
    SteadyClock::duration getStartTime() const { return mStartTime; }
    SteadyClock::duration getDuration() const { return mDuration; }

    std::shared_ptr<ActionData> getActionData() { return mActionData; }

    void setStartTime(SteadyClock::duration start);
    void setActionData(std::shared_ptr<ActionData>);

    Kind getKind() { return mKind; }
    std::list<int>* getAffected() { return &mAffectedObjects; };

    /**
     * Set type of Action: for more specific processing
     */
    void setType(std::string type) { mType = type; }
    std::string getType() { return mType; }

    void setActionId(int id) { mActionId = id; }
    int getActionId() { return mActionId; }

    void setBeginId(int id);
    int getBeginId() { return mBeginActionId; };

    void addAffected(int id);


protected:
    /**
     * Action Data contains the needed data provided for action (like transmissions)
     */
    std::shared_ptr<ActionData> mActionData = nullptr;

    /**
     * Contains all object related data
     */
    std::shared_ptr<ObjectContext> mObjectContext;

    /**
     * Time after which the gathering action should be scheduled
     */
    SteadyClock::duration mDuration;

    /**
     * Start time
     * If startTime == 0, start immediately
     */
    SteadyClock::duration mStartTime;

    /**
     * All objects which are affected by this action
     */
    std::list<int> mAffectedObjects;

private:
    Kind mKind;

    int mActionId = 0;
    int mBeginActionId = 0;

    std::string mType = "";
};

std::shared_ptr<Action> makeEndAction(std::shared_ptr<Action>);
std::shared_ptr<Action> makeEndAction(std::shared_ptr<Action>, std::list<int> endActionList);

} // ns paresis


#endif /* _ACTION_HPP_NVOS */