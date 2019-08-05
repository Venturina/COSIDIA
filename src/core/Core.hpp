#ifndef _CORE_HPP_ONSO
#define _CORE_HPP_ONSO

#include <boost/asio.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/fiber/condition_variable.hpp>
#include "core/ActionList.hpp"
#include "core/SteadyClock.hpp"
#include "objects/ObjectList.hpp"

namespace paresis
{

class Core
{
public:
    Core();

    /** Fetch the current ObjectId
     * Atomic fetch_add to increase the counter by one
     * Can be called from any Fiber!
     * @return: int ObjectId to use
     **/
    ObjectId getNextObjectId();

    /** Fetch the current ActionId
     * Atomic fetch_add to increase the counter by one
     * Can be called from any Fiber!
     * @return: int ActionId to use
     **/
    ActionId getNextActionId();

    /**
     * Schedules a new action
     * Only to be called from main Fiber!
     * @param: action Action to be scheduled
     **/
    void scheduleAction(std::shared_ptr<Action>);

    void addObject(std::shared_ptr<BaseObject>);

    ObjectContainer_ptr getCurrentObjectList() { return mObjectList.getCurrentObjectList(); };

private:
    void setup();
    void startThreads(int, std::thread::id);
    void runSimulationLoop();
    void executeActionOnFinishedTimer();
    void finishSimulation();

    SteadyClock mClock;
    boost::asio::io_service mIoService;
    boost::asio::steady_timer mTimer;
    ActionList mActions;
    ActionP mCurrentAction;

    ObjectList mObjectList;

    boost::fibers::condition_variable mConditionClose;
    bool mIsFinished = false;
    boost::fibers::mutex mFiberMutex;

    std::vector<std::thread> mThreads;

    std::atomic<long int> mCurrentActionId{0};

};

} // ns paresis


#endif /* _CORE_HPP_ONSO */