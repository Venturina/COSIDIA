#ifndef _ACTION_HPP_NVOS
#define _ACTION_HPP_NVOS

#include <list>
#include <memory>
#include "core/ActionData.hpp"
#include "objects/ObjectContext.hpp"

namespace paresis
{
    class BaseObject;
}

namespace paresis
{


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
    Action(uint32_t duration, Kind k, uint64_t start, std::shared_ptr<BaseObject>);
    uint64_t getStartTime() const { return mStartTime; }
    Kind getKind() { return mKind; }
    std::list<std::shared_ptr<BaseObject>>* getAffected() { return &mAffectedObjects; };


protected:
    /**
     * Action Data contains the needed data provided for action (like transmissions)
     */
    ActionData mActionData;

    /**
     * Contains all object related data
     */
    ObjectContext mObjectContext;

    /**
     * Time after which the gathering action should be scheduled
     */
    uint32_t mDuration;

    /**
     * Start time
     * If startTime == 0, start immediately
     */
    uint64_t mStartTime = 0;

    /**
     * All objects which are affected by this action
     */
    std::list<std::shared_ptr<BaseObject>> mAffectedObjects;

private:
    Kind mKind;
};

} // ns paresis


#endif /* _ACTION_HPP_NVOS */