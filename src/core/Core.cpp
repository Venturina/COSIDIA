#include "core/Core.hpp"
#include "fiber/customWorkStealing.hpp"
#include <boost/bind.hpp>
#include <boost/fiber/algo/work_stealing.hpp>
#include <boost/fiber/fiber.hpp>
#include <boost/fiber/operations.hpp>
#include <iostream>
#include <sstream>

#include <loguru/loguru.hpp>

#include <radio/MediumAccess.hpp>
#include <objects/SimulationManager.hpp>


namespace paresis
{

Core::Core() : mClock(SteadyClock(1)), mTimer(mIoService)
{
    setup();
    boost::asio::io_service::work work(mIoService);
    runSimulationLoop();
    mIoService.run();
}

// is this ok?
int Core::getNextObjectId()
{
    return mObjectList.getCurrentObjectId().fetch_add(1, std::memory_order::memory_order_relaxed);
}

int Core::getNextActionId()
{
    return mCurrentActionId.fetch_add(1, std::memory_order::memory_order_relaxed);
}

void Core::scheduleAction(std::shared_ptr<Action> action)
{
    mActions.insertAction(action);
}

void Core::addObject(std::shared_ptr<BaseObject> obj)
{
    mObjectList.addToObjectContainer(obj->getObjectId(), obj);
}

void Core::setup()
{
    boost::fibers::use_scheduling_algorithm<boost::fibers::algo::shared_work>();
    // spawn thread
    int concurentThreadsSupported = std::thread::hardware_concurrency();
    if(concurentThreadsSupported == 0) {
        throw(std::runtime_error("Could not detect number of cores"));
    } else {
        DLOG_F(INFO, "detected %d number of concurrent cores", concurentThreadsSupported);
    }

    for(unsigned x = 0; x < std::min(concurentThreadsSupported-2, 30 ); x++) {
        mThreads.emplace_back(&Core::startThreads, this, concurentThreadsSupported, std::this_thread::get_id());
    }

    // create dummy actions
    // for (int x = 0; x < 10000; x++)
    // {
    //     std::cout << "create action" << x  << std::endl;
    //     auto m = std::make_shared<MediumAccess>(this);
    //     Action a(std::chrono::seconds{1} + std::chrono::nanoseconds{200}, Action::Kind::START, std::chrono::seconds{1} + std::chrono::milliseconds{x}, m);
    //     mActions.insertAction(std::make_shared<Action>(a));
    // }

    auto manager = std::make_shared<SimulationManager>(this);
    mObjectList.addToObjectContainer(manager->getObjectId(), manager);
}

void Core::startThreads(int thread_count, std::thread::id mainThread) {
    // thread registers itself at work-stealing scheduler
    boost::fibers::use_scheduling_algorithm<boost::fibers::algo::shared_work>();

    std::stringstream s;
    s << std::this_thread::get_id();
    std::stringstream m;
    m << mainThread;
    DLOG_F(INFO, "launched thread: %s with main thread: %s", s.str().c_str(), m.str().c_str());

    std::unique_lock< boost::fibers::mutex > lk( mFiberMutex);
    mConditionClose.wait(lk, [this](){ return this->mIsFinished; });

    DLOG_F(INFO, "Finished thread: %s", s.str().c_str());
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
        //std::cout << mClock.getDurationUntil(mCurrentAction->getStartTime()).count() << std::endl;
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
        for(auto & elemId : *l)
        {
            auto obj = mObjectList.getCurrentObjectList();
            if (obj) {
                (*obj)[elemId]->execute(mCurrentAction);
            }
        }
        //LOG_F(INFO, "delayed by: %d nanoseconds", (mClock.getSimTimeNow() - mCurrentAction->getStartTime()).count());
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