#ifndef _CORE_HPP_ONSO
#define _CORE_HPP_ONSO

#include <boost/asio.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/fiber/condition_variable.hpp>
#include <stdlib.h>
#include "core/ActionList.hpp"
#include "core/SteadyClock.hpp"
#include "objects/ObjectList.hpp"

namespace paresis
{

class Core
{
public:
    Core(std::shared_ptr<SteadyClock>);
    Core();

    /** Fetch the current ObjectId
     * Atomic fetch_add to increase the counter by one
     * Can be called from any Fiber!
     * @return: int ObjectId to use
     **/
    int getNextObjectId();

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
    virtual void scheduleAction(std::shared_ptr<Action>);

    /** only call from main thread
     **/
    void addObject(std::shared_ptr<BaseObject>);
    void addUniqueObject(std::shared_ptr<BaseObject>);

    void removeObjectFromSimulation(int);

    std::shared_ptr<SteadyClock> getClock() { return mClock; }

    ObjectContainer_ptr getCurrentObjectList() { return mObjectList.getCurrentObjectContainer(); };

    int getRandomNumber();

protected:
    void setup();
    void startThreads(int, std::thread::id);
    void runSimulationLoop();
    void executeActionOnFinishedTimer();
    void finishSimulation();

    std::shared_ptr<SteadyClock> mClock;
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
    std::mt19937 mRnd;
    std::uniform_int_distribution<> mDistribution;
};

void setCoreP(Core*);
Core* getCoreP();

} // ns paresis


#endif /* _CORE_HPP_ONSO */