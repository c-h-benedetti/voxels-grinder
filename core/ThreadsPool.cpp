#include "ThreadsPool.hpp"
#include "Bucket.hpp"

std::unique_ptr<ThreadsPool> ThreadsPool::instance = std::make_unique<ThreadsPool>(N_THREADS);


///////////////////////////////////////////////////////////////////////////////////////////////


ThreadsPool::ThreadsPool(size_t numThreads=std::thread::hardware_concurrency()) {
    if (numThreads == 0) {
        throw unsupported_operation("Can't make a threads pool of 0 threads.");
    }
    workers.reserve(numThreads);
    for (size_t i = 0; i < numThreads; ++i)
        workers.emplace_back(
            [this] () { this->process_tasks(); }
        );
}

ThreadsPool::~ThreadsPool() {
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        stop = true;
    }
    condition.notify_all();
    for (std::thread &worker : workers)
        worker.join();
}

void ThreadsPool::delegate_task(Task* todo, const std::vector<Bucket>& buckets_list) {
    if (todo == nullptr) {
        throw unsupported_operation("Can not accept a task being nullptr.");
    }
    if (buckets_list.size() == 0) {
        throw unsupported_operation("Can not work on an empty buckets list.");
    }
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        this->buckets = buckets_list;
        this->task = todo;
        activeTasksCount = buckets_list.size();
    }
    condition.notify_all();

    std::unique_lock<std::mutex> lock(queueMutex);
    allTasksFinishedCondition.wait(lock, [this] { return activeTasksCount == 0; });  
}

ThreadsPool* ThreadsPool::threads_pool() {
    if (ThreadsPool::instance == nullptr)
        ThreadsPool::instance = std::make_unique<ThreadsPool>(N_THREADS);
    return ThreadsPool::instance.get();
}

/// Loop executed by all the workers:
void ThreadsPool::process_tasks() {
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

        this->task->execute(bucket);
        activeTasksCount--;

        if (activeTasksCount == 0) {
            allTasksFinishedCondition.notify_all();
        }
    }
}


/** WORKFLOW:
 * 
 * 1. Dans le Processor, on instancie la Task.
 * 2. On appelle directement threadspool.delegate_task() sur la task
 * 3. La fonction execute(bucket) est appelée par le worker.
 * 4. La fonction execute() appelle la fonction run() pour trouver le vrai type de l'objet et appelle le run de l'objet.
 * 5. L'objet appelle la bonne surcharge du run de la Task.
 * 
 */