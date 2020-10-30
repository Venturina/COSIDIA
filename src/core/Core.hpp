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

extern int timingBuffer[64ULL*1024ULL*1024ULL];
extern unsigned long long currId;

class Core
{
public:
    Core(std::shared_ptr<SteadyClock>);
    Core();

    /**
     *  Fetch the current ObjectId
     * Must be called from main thread.
     * @return: int ObjectId to use
     **/
    int getNextObjectId();

    /**
     * Fetch the current ActionId
     * Must be called from main thread.
     * @return: int ActionId to use
     **/
    ActionId getNextActionId();

    /**
     * Schedules a new action
     * Only to be called from main Fiber!
     * @param: action Action to be scheduled
     **/
    virtual void scheduleAction(std::shared_ptr<Action>);

    /**
     * Adds an Object to the global ObjectList
     *
     * @param Object to add is not allowed to be a unique object
     **/
    void addObject(std::shared_ptr<BaseObject>);

    /**
     * Adds an Unique Object to the global ObjectList
     *
     * @param Object to add must be a unique object
     **/
    void addUniqueObject(std::shared_ptr<BaseObject>);

    /**
     * Deletes an Object from the global Object list
     *
     *  @param ObjectId from the object which should be deleted.
     **/
    void removeObjectFromSimulation(int);

    /**
     * Allows to retrive clock information.
     *
     * @return Simulation Clock
     **/
    const SteadyClock& getClock() { return *mClock; }

    /**
     * Gives CurrentObjectList to be read from Objects
     *
     * @return Constant copy of CurrentObjectList. Can be handed to objects, is guaranteed to never change.
     **/
    const ConstObjectContainer_ptr getCurrentObjectList() { return mObjectList.getCurrentObjectContainer(); };

    /**
     * Gives a random number, is deterministic.
     *
     * @return random number
     **/
    int getRandomNumber();

    void removeAction(std::shared_ptr<Action>);

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
    ConstActionP mCurrentAction;

    ObjectList mObjectList;

    boost::fibers::condition_variable mConditionClose;
    bool mIsFinished = false;
    boost::fibers::mutex mFiberMutex;

    std::vector<std::thread> mThreads;

    int mCurrentActionId = 0;
    std::mt19937 mRnd;
    std::uniform_int_distribution<> mDistribution;
};

void setCoreP(Core*);
Core* getCoreP();

std::chrono::time_point<std::chrono::system_clock> getUtcStartTime();

std::thread::id getCoreThreadId();
bool onCoreThread();

} // ns paresis


#endif /* _CORE_HPP_ONSO */