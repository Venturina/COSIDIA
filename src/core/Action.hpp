#ifndef _ACTION_HPP_NVOS
#define _ACTION_HPP_NVOS


#include <chrono>
#include <list>
#include <memory>
#include "actionData/ActionData.hpp"
#include "core/SteadyClock.hpp"
#include "objects/ObjectContext.hpp"
#include "objects/ObjectId.hpp"
#include "core/Symbol.hpp"


namespace cosidia
{

class BaseObject;

typedef int ActionId;

/** A action describes
 *  a specific action in the cosidia system.
 *  It contains a start time point, a duration and something to execute
 */

class Action : public std::enable_shared_from_this<Action>
{
public:
    using Duration = SimClock::duration;
    using TimePoint = SimClock::time_point;

    /**
     * Get start time of an Action
     */
    TimePoint getStartTime() const { return mStartTime; }

    /**
     * Get duration of an Action
     */
    Duration getDuration() const { return mDuration; }

    /**
     * Shifts start time to the given TimePoint
     */
    void shiftStartTime(TimePoint);

    /**
     * Get End time
     */
    TimePoint getEndTime() const { return mDuration + mStartTime; }

    /**
     * Get attached ActionData, nullptr if no ActionData is attached
     */
    std::shared_ptr<const ActionData> getActionData() { return mActionData; }

    /**
     * Attach ActionData to Action
     * @param ActionData to attach, overriding of existing ActionData is not allowed
     */
    void setActionData(std::shared_ptr<const ActionData>);

    /**
     * Get affected object
     *
     * @return const reference to affected object
     */
    const ObjectId getAffected() const { return mAffectedObject; }

    /**
     * Set type string of Action: for more specific processing
     */
    void setType(const Symbol& type) { mType = type; }

    /**
     * Get type string of Action
     */
    const Symbol& getType() const { return mType; }

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
     * Returns corresponding Start Action ID
     *
     * @return 0 for Start and Init Actions
     */
    int getBeginId() { return mBeginActionId; };

    /**
     * Returns the ID of the object which generated the Action
     *
     * @return ObjectID
     */
    ObjectId getGeneratingObject() const { return mGeneratingObject; }

    /**
     * Schedule handlers in Future Action List
     * Must be defined in subclasses
     */
    virtual void scheduleStartHandler() = 0;
    virtual void scheduleEndHandler() = 0;

protected:
    /**
     * Creates an Action for a specific object
     *
     * @param duration Time span a action takes to be calculated
     * @param start Time when the action should be started
     * @param id Object which is the receiver of the Action
     * @param generating object
     */
    Action(Duration duration, TimePoint start, ObjectId id, ObjectId generator);

    /**
     * Action Data contains the needed data provided for action (like transmissions)
     */
    std::shared_ptr<const ActionData> mActionData = nullptr;

    /**
     * Contains all object related data
     */
    std::shared_ptr<ObjectContext> mObjectContext;

    /**
     * Time after which the gathering action should be scheduled
     */
    Duration mDuration;

    /**
     * Start time
     * If startTime == 0, start immediately
     */
    TimePoint mStartTime;

    /**
     * Object wich is affected by the action
     */
    ObjectId mAffectedObject;

    /**
     * ActionId's are deterministic. Each action gets the same ID in every simulation run
     */
    int mActionId = 0;

private:

    /**
     * Required for Actions with Kind=END
     * Has to contain the ID of the corresponding START Action
     */
    int mBeginActionId = 0;

    /**
     * Contains the ObjectId of the Object which has generated the Action
     */
    ObjectId mGeneratingObject;

    static const Symbol default_type;
    Symbol mType = default_type;
};

/**
 * This class must be used to create actions via factory
 */
template <class T> class ActionFactory
{
public:

    /**
     * Creates a fully constructed action
     * @param duration Time span a action takes to be calculated
     * @param start Time when the action should be started
     * @param id Object which is the receiver of the Action
     * @param generating object
     * @return shared_ptr to the created action
     */
    static std::shared_ptr<T> create(Action::Duration duration, Action::TimePoint start, ObjectId id, ObjectId generator)
    {
        auto tmp = std::shared_ptr<T>(new T { duration, start, id, generator });
        tmp->afterConstruction();
        return tmp;
    }
};


} // ns cosidia


#endif /* _ACTION_HPP_NVOS */