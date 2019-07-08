#ifndef _CORE_HPP_ONSO
#define _CORE_HPP_ONSO

#include <boost/asio.hpp>
#include "boost/asio/steady_timer.hpp"
#include <boost/fiber/condition_variable.hpp>
#include "core/ActionList.hpp"
#include "core/SteadyClock.hpp"

namespace paresis
{

class Core
{
public:
    Core();
    
private:
    void setup();
    void runSimulationLoop();
    void finishSimulation();

    void startThreads(int);

    void executeActionOnFinishedTimer();

    SteadyClock mClock;
    boost::asio::io_service mIoService;
    boost::asio::steady_timer mTimer;
    ActionList mActions;
    ActionP mCurrentAction;

    boost::fibers::condition_variable mConditionClose;
    bool mIsFinished = false;
    boost::fibers::mutex mFiberMutex;

    std::thread t;
    std::thread t2;

};

} // ns paresis


#endif /* _CORE_HPP_ONSO */