#ifndef _CORE_HPP_ONSO
#define _CORE_HPP_ONSO

#include <boost/asio.hpp>
#include <boost/fiber/condition_variable.hpp>
#include "core/ActionList.hpp"
#include "core/Clock.hpp"

namespace paresis
{
namespace core
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

    Clock mClock;
    boost::asio::io_service mIoService;
    ActionList mActions;
    ConstActionP mCurrentAction;

    boost::fibers::condition_variable mConditionClose;
    bool mIsFinished = false;
    boost::fibers::mutex mFiberMutex;

    std::thread t;
    std::thread t2;

};

} // ns core
} // ns paresis


#endif /* _CORE_HPP_ONSO */