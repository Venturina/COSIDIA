#pragma once

#include "core/SteadyClock.hpp"
#include <SQLiteCpp/Database.h>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <thread>
#include <vector>

namespace cosidia
{

class SqliteBackend
{
public:
    static SqliteBackend& get();
    void recordTiming(const char*, SimClock::time_point, std::chrono::high_resolution_clock::duration);

private:
    SqliteBackend();
    ~SqliteBackend();
    using RecordTask = std::function<void(SQLite::Statement&)>;

    void createTables();
    void runConsumer();

    std::atomic_bool mConsumerRunning;
    std::thread mConsumerThread;
    SQLite::Database mDatabase;
    std::vector<RecordTask> mQueue;
    std::mutex mQueueMutex;
    std::condition_variable mQueueChanged;
};

} // namespace cosidia
