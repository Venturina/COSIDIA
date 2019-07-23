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

    int getNextObjectId();
    int getNextActionId();

    void scheduleAction();

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