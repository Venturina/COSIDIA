#include "SqliteBackend.hpp"
#include <SQLiteCpp/Statement.h>
#include <SQLiteCpp/Transaction.h>

using namespace std::chrono;

namespace cosidia
{

SqliteBackend& SqliteBackend::get()
{
    static SqliteBackend backend;
    return backend;
}

SqliteBackend::SqliteBackend() :
    mConsumerRunning(true),
    mDatabase("cosidia.db3", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE)
{
    createTables();
    mQueue.reserve(1024);
    mConsumerThread = std::thread { &SqliteBackend::runConsumer, this };
}

SqliteBackend::~SqliteBackend()
{
    if (mConsumerThread.joinable()) {
        mConsumerRunning = false;
        mQueueChanged.notify_all();
        mConsumerThread.join();
    }
}

void SqliteBackend::createTables()
{
    mDatabase.exec("DROP TABLE IF EXISTS timing");
    mDatabase.exec("CREATE TABLE timing ( "
        "id INTEGER PRIMARY KEY, name TEXT NOT NULL, "
        "start INTEGER NOT NULL, duration INTEGER NOT NULL)");
}

void SqliteBackend::recordTiming(const char* name, SimClock::time_point sim_start, high_resolution_clock::duration duration)
{
    RecordTask task = [=, name_str = std::string { name }](SQLite::Statement& statement) {
        statement.bindNoCopy(":name", name_str);
        statement.bind(":start", sim_start.time_since_epoch().count());
        statement.bind(":duration", duration_cast<nanoseconds>(duration).count());
    };

    bool flush = false;
    {
        std::unique_lock<std::mutex> lock { mQueueMutex };
        mQueue.emplace_back(std::move(task));
        flush = mQueue.size() > mQueue.capacity() / 2;
    }

    if (flush) {
        mQueueChanged.notify_one();
    }
}

void SqliteBackend::runConsumer()
{
    std::vector<RecordTask> queue;
    queue.reserve(1024);

    SQLite::Statement statement { mDatabase, "INSERT INTO timing VALUES (NULL, :name, :start, :duration)" };

    while (mConsumerRunning)
    {
        std::unique_lock<std::mutex> lock(mQueueMutex);
        mQueueChanged.wait(lock, [this]() { return (mQueue.size() > 0 || !mConsumerRunning); });

        std::swap(queue, mQueue);
        lock.unlock();

        SQLite::Transaction transaction(mDatabase);
        for (auto& task : queue)
        {
            task(statement);
            statement.exec();
            statement.reset();
        }
        transaction.commit();
        queue.clear();
    }
}

} // namespace cosidia
