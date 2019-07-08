#include "core/Core.hpp"
#include <boost/bind.hpp>
#include <boost/fiber/algo/work_stealing.hpp>
#include <boost/fiber/fiber.hpp>
#include <boost/fiber/operations.hpp>
#include <iostream>
#include <sstream>

#include <radio/MediumAccess.hpp>


namespace paresis
{
void Core::startThreads(int thread_count) {
    // thread registers itself at work-stealing scheduler
    boost::fibers::use_scheduling_algorithm< boost::fibers::algo::work_stealing >( thread_count);

    std::stringstream str;
    str << "launched thread: " << std::this_thread::get_id();
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
    mIoService.run();
    runSimulationLoop();
}

void Core::setup()
{
    int threadCount = 3;
    t = std::thread(&Core::startThreads, this, threadCount);
    t2 = std::thread(&Core::startThreads, this, threadCount);
    boost::fibers::use_scheduling_algorithm< boost::fibers::algo::work_stealing >( threadCount);
    // spawn thread
        //threads.emplace_back( thread, 4);

    //std::shared_ptr<MediumAccess> m;

    auto medium = std::make_shared<MediumAccess>();
    auto medium1 = std::make_shared<MediumAccess>();
    auto medium2 = std::make_shared<MediumAccess>();

    Action a(uint32_t(1e+9), Action::Kind::START, uint64_t(1e+9), medium);
    Action b(uint32_t(1e+9), Action::Kind::START, uint64_t(1e+9), medium1);
    Action c(uint32_t(1e+9), Action::Kind::START, uint64_t(1e+9), medium2);

    mActions.insertAction(std::make_shared<Action>(a));
    mActions.insertAction(std::make_shared<Action>(b));
    mActions.insertAction(std::make_shared<Action>(c));

}

void Core::runSimulationLoop()
{
    mCurrentAction = mActions.getNextAction();
    if(!mCurrentAction) {
        //sleep(5);
        finishSimulation();
    } else {
        std::cout << "pick next action" << std::endl;
        mTimer.expires_after(mClock.getDurationUntil(mCurrentAction->getStartTime()));
        mTimer.async_wait(boost::bind(&Core::executeActionOnFinishedTimer, this));
        if(mIoService.stopped()) {
            mIoService.restart();
            mIoService.run();
        }
    }
}

void Core::executeActionOnFinishedTimer()
{
    std::cout << "execute" << std::endl;
    if(!(mCurrentAction == mActions.getNextAction())) {
        throw std::runtime_error("messed up with upcoming tasks");
    } else {
        auto l = mCurrentAction->getAffected();
        for(auto & elem : *l)
        {
            elem->execute(mCurrentAction);
        }
        if(mCurrentAction->getDuration() > 0) {
            mActions.insertAction(std::make_shared<Action>(0, Action::Kind::END, mCurrentAction->getStartTime() + mCurrentAction->getDuration(), std::move(*(mCurrentAction->getAffected()))));
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
    t.join();
    t2.join();
}


} // ns paresis