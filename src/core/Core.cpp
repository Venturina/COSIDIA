#include "core/Core.hpp"
#include "boost/asio/high_resolution_timer.hpp"
#include <boost/bind.hpp>
#include <boost/fiber/algo/work_stealing.hpp>
#include <boost/fiber/fiber.hpp>
#include <boost/fiber/operations.hpp>
#include <iostream>


namespace paresis
{
namespace core
{

void Core::startThreads(int thread_count) {
    // thread registers itself at work-stealing scheduler
    boost::fibers::use_scheduling_algorithm< boost::fibers::algo::work_stealing >( thread_count);
    std::thread::id my_thread = std::this_thread::get_id();
    std::cout << "launched thread: " << my_thread << std::endl;
    std::unique_lock< boost::fibers::mutex > lk( mFiberMutex);
    mConditionClose.wait(lk, [this](){ return this->mIsFinished; });
    std::cout << "finished thread" << my_thread << std::endl;
}

Core::Core() : mClock(Clock(1))
{
    setup();
    mIoService.run();
    runSimulationLoop();
}

void Core::setup()
{
    t = std::thread(&Core::startThreads, this, 2);
    t2 = std::thread(&Core::startThreads, this, 2);
    // spawn thread
        //threads.emplace_back( thread, 4);

    // create first task!
}

void Core::runSimulationLoop()
{
    mCurrentAction.reset(mActions.getNextAction().get());
    if(!mCurrentAction) {
        sleep(5);
        finishSimulation();
    } else {
        boost::asio::high_resolution_timer t(mIoService, mClock.getTimePointforSimTime(mCurrentAction->getStartTime()));
        t.async_wait(boost::bind(&Core::executeActionOnFinishedTimer, this));
    }
}

void Core::executeActionOnFinishedTimer()
{
    if(!(mCurrentAction == mActions.getNextAction())) {
        throw std::runtime_error("messed up with upcoming tasks");
    } else {

    }
}

void Core::finishSimulation()
{
    std::cout << "simulaton finished" << std::endl;
    {
        std::unique_lock<boost::fibers::mutex>lk(mFiberMutex);
        mIsFinished = true;
    }
    mConditionClose.notify_all();
    t.join();
    t2.join();
}


} // ns core
} // ns paresis