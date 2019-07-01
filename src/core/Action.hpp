#ifndef _ACTION_HPP_NVOS
#define _ACTION_HPP_NVOS

#include <list>
#include <memory>
#include "core/ActionData.hpp"

class BaseObject;

namespace paresis
{
namespace core
{

/** A action describes a specific action in the paresis system.
 * It contains a start time point, a duration and something to execute
 * 
 */

class Action
{
public:
    Action(uint32_t duration) : mDuration(duration) {}


protected:
    /**
     * Action Data contains the needed data provided for action (like transmissions)
     */
    ActionData mActionData;

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
};


} // ns core
} // ns paresis


#endif /* _ACTION_HPP_NVOS */