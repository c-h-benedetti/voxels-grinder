#ifndef THREAD_POOL_HPP_INCLUDED
#define THREAD_POOL_HPP_INCLUDED

#include <vector>
#include <memory>
#include <cstdlib>
#include <functional>
#include <condition_variable>
#include <thread>
#include <mutex>
#include <atomic>

#include "Task.hpp"
#include "utils/general.hpp"


class ThreadsPool {

    std::vector<std::thread> workers;
    std::mutex queueMutex;
    std::condition_variable condition_run;
    std::condition_variable condition_done;
    bool stop = false;
    size_t nextBucket = 0;
    std::atomic<size_t> activeThreads = std::atomic<size_t>(0);
    std::atomic<size_t> completedTasks = std::atomic<size_t>(0);

    Task* task;
    std::vector<Bucket> buckets;
    std::vector<int> return_codes;
    
    static std::unique_ptr<ThreadsPool> instance;

public:

    ThreadsPool() = delete;
    ThreadsPool(const ThreadsPool&) = delete;
    ThreadsPool& operator=(const ThreadsPool&) = delete;

    ThreadsPool(size_t numThreads);
    ~ThreadsPool();

    void delegate_task(Task* todo, const std::vector<Bucket>& buckets_list);
    inline size_t n_workers() const { return this->workers.size(); }
    bool success() const;
    inline float completion() const { return static_cast<float>(this->completedTasks) / static_cast<float>(this->buckets.size()); }

private:

    void kill();
    void process_tasks();

public:

    static ThreadsPool* threads_pool(size_t numThreads=std::thread::hardware_concurrency());

};

#endif //THREAD_POOL_HPP_INCLUDED


/*

class ThreadsPool {

    std::vector<std::thread> workers; // List of threads that will execute the code
    std::mutex queueMutex; // mutex to block when picking a bucket.
    std::condition_variable condition;
    bool stop = false; // Should we kill the threads and destroy the object.
    std::condition_variable allTasksFinishedCondition;
    std::atomic<int> activeTasksCount = std::atomic<int>(0);

    Task* task; // Task object that will be executed. Must be configured.
    std::vector<Bucket> buckets; // Buckets into which input has been cut.
    std::vector<int> return_codes;
    std::atomic<int> end_of_workers = std::atomic<int>(0);
    std::condition_variable allWorkersKilled;
    
    static std::unique_ptr<ThreadsPool> instance;

public:

    inline size_t n_workers() const { return this->workers.size(); }

    ThreadsPool(size_t numThreads);
    ~ThreadsPool();

    void delegate_task(Task* todo, const std::vector<Bucket>& buckets_list);

    static ThreadsPool* threads_pool(size_t numThreads=std::thread::hardware_concurrency());

    void kill();

    inline int workers_ended() const { return this->end_of_workers; }

private:

    void process_tasks();
};

*/