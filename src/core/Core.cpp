#include "core/Core.hpp"
#include "fiber/customWorkStealing.hpp"
#include <boost/bind.hpp>
#include <boost/fiber/algo/work_stealing.hpp>
#include <boost/fiber/fiber.hpp>
#include <boost/fiber/operations.hpp>
#include <iostream>
#include <sstream>

#include <radio/MediumAccess.hpp>


namespace paresis
{
void Core::startThreads(int thread_count, std::thread::id mainThread) {
    // thread registers itself at work-stealing scheduler
    boost::fibers::use_scheduling_algorithm<boost::fibers::algo::shared_work>();
    std::stringstream str;
    str << "launched thread: " << std::this_thread::get_id() << " with main thread: " << mainThread;
    std::cout <<  str.str() << std::endl;
    std::unique_lock< boost::fibers::mutex > lk( mFiberMutex);
    mConditionClose.wait(lk, [this](){ return this->mIsFinished; });
    str.str("");
    str << "finished thread: " << std::this_thread::get_id();
    std::cout << str.str() << std::endl;
}

Core::Core() : mClock(SteadyClock(1)), mTimer(mIoService)
{
    setup();
    boost::asio::io_service::work work(mIoService);
    runSimulationLoop();
    mIoService.run();
}

int Core::getNextActionId()
{
    return ++mCurrentActionId;
}

void Core::setup()
{
    int threadCount = 5;
    // t = std::thread(&Core::startThreads, this, threadCount, std::this_thread::get_id());
    // t2 = std::thread(&Core::startThreads, this, threadCount, std::this_thread::get_id());
    // t3 = std::thread(&Core::startThreads, this, threadCount, std::this_thread::get_id());
    // t4 = std::thread(&Core::startThreads, this, threadCount, std::this_thread::get_id());
    boost::fibers::use_scheduling_algorithm<boost::fibers::algo::shared_work>();
    // spawn thread
    unsigned concurentThreadsSupported = std::thread::hardware_concurrency();
    if(concurentThreadsSupported == 0) {
        throw(std::runtime_error("Could not detect number of cores"));
    } else {
        std::stringstream str;
        str << "detected " << concurentThreadsSupported << " concurrent cores";
        std::cout <<  str.str() << std::endl;
    }

    for(unsigned x = 0; x < concurentThreadsSupported-1; x++) {
        mThreads.emplace_back(&Core::startThreads, this, concurentThreadsSupported, std::this_thread::get_id());
    }

    //std::shared_ptr<MediumAccess> m;

    for (int x = 0; x < 200; x++)
    {
        auto m = std::make_shared<MediumAccess>(this);
        Action a(std::chrono::nanoseconds{int(3e+9)}, Action::Kind::START, std::chrono::nanoseconds{int(1e+9 + 100000*x)}, m);
        mActions.insertAction(std::make_shared<Action>(a));
    }



    // auto medium = std::make_shared<MediumAccess>();
    // auto medium1 = std::make_shared<MediumAccess>();
    // auto medium2 = std::make_shared<MediumAccess>();

    // Action a(uint64_t(3e+9), Action::Kind::START, uint64_t(1e+9), medium);
    // Action b(uint64_t(3e+9), Action::Kind::START, uint64_t(2e+9), medium1);
    // Action c(uint64_t(3e+9), Action::Kind::START, uint64_t(3e+9), medium2);

    // mActions.insertAction(std::make_shared<Action>(a));
    // mActions.insertAction(std::make_shared<Action>(b));
    // mActions.insertAction(std::make_shared<Action>(c));
}

void Core::runSimulationLoop()
{
    mCurrentAction = mActions.getNextAction();
    if(!mCurrentAction) {
        //sleep(5);
        finishSimulation();
    } else {
        //std::cout << "next action in: ";
        //auto t = mClock.getDurationUntil(mCurrentAction->getStartTime());
        auto expiry = mClock.getDurationUntil(mCurrentAction->getStartTime());
        mTimer.expires_after(mClock.getDurationUntil(mCurrentAction->getStartTime()));
        mTimer.async_wait(boost::bind(&Core::executeActionOnFinishedTimer, this));
        //std::cout << (int64_t)mClock.getSimTimeNow() - (int64_t)mCurrentAction->getStartTime() << std::endl;
    }
}

void Core::executeActionOnFinishedTimer()
{
    if(!(mCurrentAction == mActions.getNextAction())) {
        throw std::runtime_error("messed up with upcoming tasks");
    } else {
        auto l = mCurrentAction->getAffected();
        for(auto & elem : *l)
        {
            elem->execute(mCurrentAction);
        }
        std::cout << (mClock.getSimTimeNow() - mCurrentAction->getStartTime()).count() << std::endl;
        if(mCurrentAction->getKind() == Action::Kind::START && mCurrentAction->getDuration() > std::chrono::nanoseconds{0}) {
            mActions.insertAction(std::make_shared<Action>(std::chrono::nanoseconds{0}, Action::Kind::END, mCurrentAction->getStartTime() + mCurrentAction->getDuration(), std::move(*(mCurrentAction->getAffected()))));
        }
        mActions.popNextAction();
        runSimulationLoop();
    }
}

void Core::finishSimulation()
{
    std::cout << "simulaton finished" << std::endl;
    {
        std::unique_lock<boost::fibers::mutex>lk(mFiberMutex);
        mIsFinished = true;
    }
    mIoService.stop();
    mConditionClose.notify_all();
    for(auto& t : mThreads) {
        t.join();
    }
}


} // ns paresis