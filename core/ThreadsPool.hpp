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
#include "general.hpp"


class ThreadsPool {

    std::vector<std::thread> workers; // List of threads that will execute the code
    std::mutex queueMutex; // mutex to block when taking a new bucket.
    std::condition_variable condition;
    bool stop = false; // Should we kill the threads and destroy the object.
    std::condition_variable allTasksFinishedCondition;
    std::atomic<int> activeTasksCount = std::atomic<int>(0);

    Task* task; // Task object that will be executed. Must be configured.
    std::vector<Bucket> buckets; // Buckets into which input has been cut.
    
    static std::unique_ptr<ThreadsPool> instance;

public:

    ThreadsPool(size_t numThreads);
    ~ThreadsPool();

    void delegate_task(Task* todo, const std::vector<Bucket>& buckets_list);

    static ThreadsPool* threads_pool();

private:

    void process_tasks();
};

#endif //THREAD_POOL_HPP_INCLUDED


/** TODO:
 * 
 * - [ ] Créer un tableau de int qui collecte les codes de retour de chaque thread.
 * 
 */