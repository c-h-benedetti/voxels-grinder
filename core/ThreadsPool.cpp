#include "ThreadsPool.hpp"
#include "Bucket.hpp"

std::unique_ptr<ThreadsPool> ThreadsPool::instance = nullptr;


///////////////////////////////////////////////////////////////////////////////////////////////


ThreadsPool::ThreadsPool(size_t numThreads) {
    if (numThreads == 0) {
        throw unsupported_operation("Can't make a threads pool of 0 threads.");
    }
    workers.reserve(numThreads);
    for (size_t i = 0; i < numThreads; i++) {
        workers.emplace_back(
            [this] () { 
                this->activeThreads++;
                this->process_tasks();
                this->activeThreads--;
            }
        );
    }
}


ThreadsPool::~ThreadsPool() {
    this->kill();
}


void ThreadsPool::kill() {
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        stop = true;
    }

    condition_run.notify_all();
    for (std::thread &worker : workers)
        worker.join();
    
    workers.clear();
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
        this->return_codes = std::vector<int>(buckets_list.size(), 0);
        this->task = todo;
        this->nextBucket = 0;
        this->completedTasks = 0;
    }
    condition_run.notify_all();
    
    std::unique_lock<std::mutex> lock(this->queueMutex);
    this->condition_done.wait(lock, [this] { return (this->nextBucket >= this->buckets.size()) && (this->completedTasks >= this->buckets.size()); });  
}


bool ThreadsPool::success() const {
    for (size_t i = 0 ; i < this->return_codes.size() ; i++) {
        if (this->return_codes[i] != 0) { return false; }
    }
    return true;
}


ThreadsPool* ThreadsPool::threads_pool(size_t numThreads) {
    if (ThreadsPool::instance == nullptr)
        ThreadsPool::instance = std::make_unique<ThreadsPool>(numThreads);
    return ThreadsPool::instance.get();
}

/// Loop executed by all the workers:
void ThreadsPool::process_tasks() {
    size_t index;
    Bucket bucket;
    while (true) {
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            this->condition_run.wait(lock, [this] { return this->stop || this->nextBucket < this->buckets.size(); });
            if (this->stop || this->nextBucket >= this->buckets.size()) { return; }
            index = this->nextBucket++;
        }

        bucket = this->buckets[index];
        try {
            this->return_codes[index] = this->task->execute(bucket);
        } catch (...) {
            this->return_codes[index] = 1;
        }
        
        this->completedTasks++;

        if (this->completedTasks >= this->buckets.size()) {
            this->condition_done.notify_all();
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