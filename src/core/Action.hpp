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
 *  It contains a start time point, a duration and something to execute
 */

class Action
{
public:
    enum class Kind {
        INIT,
        START,
        END
    };

    /**
     * Creates an Action for a specific object
     *
     * @param duration Time span a action takes to be calculated
     * @param k Kind of Action (Init, Start, End)
     * @param start Time when the action should be started
     * @param id Object which is the receiver of the Action
     */
    Action(SteadyClock::duration duration, Kind k, SteadyClock::duration start, int id);

    /**
     * Creates an Action for a list of object
     *
     * @param duration Time span a action takes to be calculated
     * @param k Kind of Action (Init, Start, End)
     * @param start Time when the action should be started
     * @param ids Objects which receive the Action
     */
    Action(SteadyClock::duration duration, Kind k, SteadyClock::duration start, std::list<int> ids);

    /**
     * Get start time of an Action
     */
    SteadyClock::duration getStartTime() const { return mStartTime; }

    /**
     * Get duration of an Action
     */
    SteadyClock::duration getDuration() const { return mDuration; }


    /**
     * Get End time
     */
    const SteadyClock::duration getEndTime() const { return mDuration + mStartTime; }

    /**
     * Get attached ActionData, nullptr if no ActionData is attached
     */
    std::shared_ptr<ActionData> getActionData() { return mActionData; }

    /**
     * Set start time of Action
     * @param start The start Time
     */
    void setStartTime(SteadyClock::duration start);

    /**
     * Attach ActionData to Action
     * @param ActionData to attach, overriding of existing ActionData is not allowed
     */
    void setActionData(std::shared_ptr<ActionData>);

    /**
     * Get Kind of Action
     */
    Kind getKind() const { return mKind; }

    /**
     * Get all affected Objects
     *
     * @return const reference to List of all Affected Objects
     */
    const std::list<int>& getAffected() const { return mAffectedObjects; }

    /**
     * Set type string of Action: for more specific processing
     */
    void setType(std::string type) { mType = type; }

    /**
     * Get type string of Action
     */
    std::string getType() { return mType; }

    /**
     * Set ID of Action
     *
     * @param Id of Action, can be overridden
     */
    void setActionId(int id) { mActionId = id; }

    /**
     * Get id of Action
     */
    int getActionId() const { return mActionId; }

    /**
     * Required for Actions with Kind=END
     * Has to correspond to the ActionId of Start Action
     */
    void setBeginId(int id);

    /**
     * Returns corresponding Start Action ID
     *
     * @return 0 for Start and Init Actions
     */
    int getBeginId() { return mBeginActionId; };

    /**
     * Adds additional affected objects by ID
     *
     * @param id of the affected Object
     */
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

    /**
     * Required for Actions with Kind=END
     * Has to contain the ID of the corresponding START Action
     */
    int mBeginActionId = 0;

    std::string mType = "default";
};

std::shared_ptr<Action> makeEndAction(std::shared_ptr<Action>);
std::shared_ptr<Action> makeEndAction(std::shared_ptr<Action>, std::list<int> endActionList);

} // ns paresis


#endif /* _ACTION_HPP_NVOS */