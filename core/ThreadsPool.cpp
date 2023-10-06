#include "ThreadsPool.hpp"

std::unique_ptr<ThreadPool> ThreadPool::instance = std::make_unique<ThreadPool>(N_THREADS);

ThreadPool::ThreadPool(size_t numThreads) {
    workers.reserve(numThreads);
    for (size_t i = 0; i < numThreads; ++i)
        workers.emplace_back(
            [this] () { this->process_tasks(); }
        );
}

ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        stop = true;
    }
    condition.notify_all();
    for (std::thread &worker : workers)
        worker.join();
}

void ThreadPool::delegate_task(std::function<bool(Bucket)> todo, std::vector<Bucket>&& buckets_list) {
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        this->buckets = buckets_list;
        this->task = todo;
        activeTasksCount = buckets_list.size();  // Increment the active tasks count
    }
    condition.notify_all();

    std::unique_lock<std::mutex> lock(queueMutex);
    allTasksFinishedCondition.wait(lock, [this] { return activeTasksCount == 0; });  
}

ThreadPool* ThreadPool::threadpool() {
    if (ThreadPool::instance == nullptr)
        ThreadPool::instance = std::make_unique<ThreadPool>(N_THREADS);
    return ThreadPool::instance.get();
}


void ThreadPool::process_tasks() {
    while (true) {
        Bucket bucket;
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            condition.wait(lock, [this] { return this->stop || !this->buckets.empty(); });
            if (this->stop && this->buckets.empty())
                return;
            bucket = this->buckets.back();
            this->buckets.pop_back();
        }

        this->task(bucket);

        activeTasksCount--;
        if (activeTasksCount == 0) {
            allTasksFinishedCondition.notify_all();
        }
    }
}