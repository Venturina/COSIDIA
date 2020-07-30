#include "core/Core.hpp"
#include "guiConnector/gui.hpp"
#include "fiber/customWorkStealing.hpp"
#include "utils/enforce.hpp"
#include <boost/bind.hpp>
#include <boost/fiber/algo/work_stealing.hpp>
#include <boost/fiber/fiber.hpp>
#include <boost/fiber/operations.hpp>
#include <iostream>
#include <sstream>
#include <thread>

#include <loguru/loguru.hpp>

#include <radio/MediumAccess.hpp>
#include <objects/SimulationManager.hpp>

#include "utils/invariant.hpp"


namespace paresis
{

thread_local Core* coreP;
static std::thread::id theMainThread;

int timingBuffer[64ULL*1024ULL*1024ULL];
unsigned long long currId = 0;

void setCoreP(Core* p)
{
    invariant(std::this_thread::get_id() == theMainThread, "Core set on wrong thread");
    coreP = p;
}

Core* getCoreP()
{
    invariant(std::this_thread::get_id() == theMainThread, "Core get on wrong thread");
    return coreP;
}

Core::Core() : mTimer(mIoService)
{
    theMainThread = std::this_thread::get_id();
    setCoreP(this);
}

Core::Core(std::shared_ptr<SteadyClock> clock) : mTimer(mIoService), mRnd(100), mDistribution(1, 1000)
{
    theMainThread = std::this_thread::get_id();
    setCoreP(this);

    setupGuiConnector(mIoService);

    if(!clock) {
        mClock = std::make_shared<SteadyClock>(1);
    } else {
        mClock = clock;
    }
    setup();
    // https://en.cppreference.com/w/cpp/numeric/random/uniform_int_distribution
    boost::asio::io_service::work work(mIoService);
    runSimulationLoop();
    mIoService.run();
}

int Core::getRandomNumber()
{
    return mDistribution(mRnd);
}

// is this ok?
int Core::getNextObjectId()
{
    return mObjectList.getCurrentObjectId().fetch_add(1, std::memory_order::memory_order_relaxed);
}

int Core::getNextActionId()
{
    return ++mCurrentActionId;
}

void Core::scheduleAction(std::shared_ptr<Action> action)
{
    enforce(action->getActionId() == 0, "Core: tried to schedule Action with Id already set");
    action->setActionId(getNextActionId());
    mActions.insertAction(std::move(action));
}

void Core::addObject(std::shared_ptr<BaseObject> obj)
{
    enforce(obj->isInitialized(), "Core: Object to add not initialized");
    mObjectList.addToObjectContainer(obj->getObjectId(), obj);
}

void Core::addUniqueObject(std::shared_ptr<BaseObject> obj)
{
    enforce(obj->isInitialized(), "Core: Object to add not initialized");
    mObjectList.addUnique(obj);
}

void Core::removeObjectFromSimulation(int id) {
    mObjectList.removeFromSimulation(id);
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

    auto manager = std::make_shared<SimulationManager>();
    mObjectList.addToObjectContainer(manager->getObjectId(), manager);
}

void Core::startThreads(int thread_count, std::thread::id mainThread) {
    // thread registers itself at work-stealing scheduler
    boost::fibers::use_scheduling_algorithm<boost::fibers::algo::shared_work>();

    std::stringstream s;
    s << std::this_thread::get_id();
    std::stringstream m;
    m << mainThread;
    LOG_F(INFO, "launched thread: %s with main thread: %s", s.str().c_str(), m.str().c_str());

    std::unique_lock< boost::fibers::mutex > lk( mFiberMutex);
    mConditionClose.wait(lk, [this](){ return this->mIsFinished; });

    LOG_F(INFO, "Finished thread: %s", s.str().c_str());
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
        auto expiry = mClock->getDurationUntil(mCurrentAction->getStartTime());
        //std::cout << mClock.getDurationUntil(mCurrentAction->getStartTime()).count() << std::endl;
        mTimer.expires_after(mClock->getDurationUntil(mCurrentAction->getStartTime()));
        mTimer.async_wait(boost::bind(&Core::executeActionOnFinishedTimer, this));
        //std::cout << (int64_t)mClock.getSimTimeNow() - (int64_t)mCurrentAction->getStartTime() << std::endl;
    }
}

void Core::executeActionOnFinishedTimer()
{
    if(!(mCurrentAction == mActions.getNextAction())) {
        throw std::runtime_error("messed up with upcoming tasks");
    } else {
        enforce(mCurrentAction->getActionId()!=0, "Core: tried to execute Action without Action Id");
        auto l = mCurrentAction->getAffected();
        std::list<int> endActionList;
        for(auto & elemId : *l)
        {
            enforce(elemId >= 0, "Core: tried to execute object with id 0");
            auto obj = mObjectList.getObjectByIdFromCurrentContainer(elemId);
            if (obj) {
                if(obj->execute(mCurrentAction) != 0) {
                    endActionList.push_back(elemId);
                }
            } else {
                std::cout << "test" << std::endl;
            }
        }
        if(!endActionList.empty()) {
            scheduleAction(makeEndAction(mCurrentAction, endActionList));
        }
        //LOG_F(INFO, "delayed by: %d nanoseconds", (mClock.getSimTimeNow() - mCurrentAction->getStartTime()).count());
        mActions.popNextAction();
        runSimulationLoop();
    }
}

void Core::finishSimulation()
{
    LOG_F(INFO, "simulaton finished");
    {
        std::unique_lock<boost::fibers::mutex>lk(mFiberMutex);
        mIsFinished = true;
    }

//     unsigned long long timingBuffer[64ULL*1024ULL*1024ULL];
// unsigned long long currId = 0;
    for(unsigned long long id = 0; id < currId; id ++) {
        std::cout << timingBuffer[id] <<  std::endl;
    }
    mIoService.stop();
    mConditionClose.notify_all();
    for(auto& t : mThreads) {
        t.join();
    }
}


} // ns paresis