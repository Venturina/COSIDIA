#include "core/Core.hpp"
#include "guiConnector/gui.hpp"
#include "fiber/customWorkStealing.hpp"
#include "utils/enforce.hpp"
#include <boost/bind/bind.hpp>
#include <boost/fiber/algo/work_stealing.hpp>
#include <boost/fiber/fiber.hpp>
#include <boost/fiber/operations.hpp>
#include <boost/format.hpp>
#include <thread>

#include <loguru/loguru.hpp>

#include <radio/MediumAccess.hpp>
#include <objects/SimulationManager.hpp>

#include "utils/invariant.hpp"


namespace cosidia
{

thread_local Core* coreP;
static std::thread::id theMainThread;

int timingBuffer[64ULL*1024ULL*1024ULL];
unsigned long long currId = 0;

std::chrono::time_point<std::chrono::system_clock> utcStartTime;
bool timeIsSet;

std::chrono::time_point<std::chrono::system_clock> getUtcStartTime()
{
    enforce(timeIsSet, "Core: UTC Start time not valid");
    return utcStartTime;
}

void setUtcStartTime()
{
    enforce(!timeIsSet, "Core: tried to set start time twice");
    enforce(onCoreThread(), "Core: required to be on core for setting time")
    utcStartTime = std::chrono::system_clock::now();
    timeIsSet = true;
}

bool onCoreThread()
{
    return std::this_thread::get_id() == theMainThread;
}

std::thread::id getCoreThreadId()
{
    return theMainThread;
}

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

Core::Core() : mTimer(mIoService), mActions(mCurrentActionTime)
{
    theMainThread = std::this_thread::get_id();
    setCoreP(this);
}

Core::Core(std::shared_ptr<SteadyClock> clock) : mTimer(mIoService), mRnd(100), mDistribution(1, 1000), mActions(mCurrentActionTime)
{
    theMainThread = std::this_thread::get_id();
    setCoreP(this);

    setupGuiConnector(mIoService);

    if(!clock) {
        mClock = std::make_shared<SteadyClock>(1);
    } else {
        mClock = clock;
    }

    setUtcStartTime();

    setup();
    // https://en.cppreference.com/w/cpp/numeric/random/uniform_int_distribution
    boost::asio::io_service::work work(mIoService);
    runSimulationLoop();
    mIoService.run();
}

int Core::getRandomNumber()
{
    enforce(onCoreThread() == true, "Core: getRandomNumber only allowed to be called from core");
    return mDistribution(mRnd);
}

ObjectId Core::getNextObjectId()
{
    enforce(onCoreThread() == true, "Core: Object ID's must be fetched from core!");
    return mObjectList.getNextObjectId();
}

int Core::getNextActionId()
{
    enforce(onCoreThread() == true, "Core: Action ID's must be fetched from core!");
    return ++mCurrentActionId;
}

void Core::scheduleAction(std::shared_ptr<ActionHandler> handler)
{
    handler->getAction()->setActionId(getNextActionId());
    mActions.insertHandler(handler);
}

void Core::addObject(std::shared_ptr<BaseObject> obj)
{
    enforce(onCoreThread() == true, "Core: Objects must be added from core!");
    enforce(obj->isInitialized(), "Core: Object to add not initialized");
    mObjectList.addToObjectContainer(obj->getObjectId(), obj);
}

void Core::addUniqueObject(std::shared_ptr<BaseObject> obj)
{
    enforce(onCoreThread() == true, "Core: Objects must be added from core!");
    enforce(obj->isInitialized(), "Core: Object to add not initialized");
    mObjectList.addUnique(obj);
}

void Core::removeObjectFromSimulation(ObjectId id) {
    enforce(onCoreThread() == true, "Core: Objects must be removed from core!");
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

    int workerThreads = std::min(concurentThreadsSupported - 2, 30);
    for(unsigned x = 1; x <= workerThreads; ++x) {
        mThreads.emplace_back(&Core::startThread, this, x, workerThreads);
    }

    auto manager = std::make_shared<SimulationManager>();
    mObjectList.addToObjectContainer(manager->getObjectId(), manager);
}

void Core::startThread(int worker_this, int worker_total) {
    // thread registers itself at work-stealing scheduler
    boost::fibers::use_scheduling_algorithm<boost::fibers::algo::shared_work>();

    boost::format thread_name_fmt("worker %1%/%2%");
    thread_name_fmt % worker_this % worker_total;
    const char* thread_name = thread_name_fmt.str().c_str();
    loguru::set_thread_name(thread_name);

    LOG_F(INFO, "Launched thread: %s", thread_name);

    std::unique_lock< boost::fibers::mutex > lk( mFiberMutex);
    mConditionClose.wait(lk, [this](){ return this->mIsFinished; });

    LOG_F(INFO, "Finished thread: %s", thread_name);
}

void Core::runSimulationLoop()
{
    enforce(mActions.getNextTimePoint() >= mCurrentActionTime, "Core: wrong order of actions");
    mCurrentActionTime = mActions.getNextTimePoint();

    if(mCurrentActionTime == SimClock::invalid()) {
        //sleep(5);
        finishSimulation();
    } else {
        mTimer.expires_after(mClock->getDurationUntil(mCurrentActionTime));
        mTimer.async_wait(boost::bind(&Core::executeActionOnFinishedTimer, this));
    }
}

void Core::executeActionOnFinishedTimer()
{
    auto handlers = mActions.popNextHandlers();
    enforce(mCurrentActionTime == handlers.begin()->get()->getTime(), "Core: currentAction time corresponds not to fetched action time");
    for(auto& handler : handlers) {
        enforce(handler->getAction()->getActionId() != 0, "Core: tried to execute Action without Id");

        auto affected = handler->getAction()->getAffected();
        enforce(affected.valid(), "Core: tried to execute object with invalid id");

        auto obj = mObjectList.getObjectByIdFromCurrentContainer(affected);
        if (obj) {
            handler->invoke(obj.get());
        } else {
            DLOG_F(INFO, "Core: dropped Action for deleted Object: %d", affected);
        }
    }

    runSimulationLoop();
}

void Core::finishSimulation()
{
    LOG_F(INFO, "simulation finished");
    {
        std::unique_lock<boost::fibers::mutex>lk(mFiberMutex);
        mIsFinished = true;
    }

    for(unsigned long long id = 0; id < currId; id ++) {
        std::cout << timingBuffer[id] <<  std::endl;
    }
    mIoService.stop();
    mConditionClose.notify_all();
    for(auto& t : mThreads) {
        t.join();
    }
}

void Core::removeAction(std::shared_ptr<Action> action) {
    //TODO Removing Actions not allowed yet
    enforce(false, "Core: removing Actions is not implemented")
}


} // ns cosidia