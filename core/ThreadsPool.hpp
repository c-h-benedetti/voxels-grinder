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

#include "Bucket.hpp"
#include "general.hpp"

/**
 * - Ajouter un moyen de kill les workers dans le le threads pool.
 * - Ajouter un tableau qui indique quels threads sont occupés et lesquels sont libres.
 * - Ajouter un booléen qui indique si un des threads a rencontré une erreur.
 * - Le ThreadPool doit pouvoir être summon en une méthode, cepdant, cette fonction doit décomposer le process en 3 phases:
 *     - Preprocess: single thread.
 *     - Process: délègue des fonctions au thread pool.
 *     - Postprocess: single thread qui permet d'assemble les résultats des threads.
*/


class ThreadPool {

    std::vector<std::thread> workers;
    std::function<bool(Bucket)> task;
    std::vector<Bucket> buckets;
    std::mutex queueMutex;
    std::condition_variable condition;
    bool stop = false;
    static std::unique_ptr<ThreadPool> instance;
    std::condition_variable allTasksFinishedCondition;
    std::atomic<int> activeTasksCount = std::atomic<int>(0);

public:

    ThreadPool(size_t numThreads);

    ~ThreadPool();

    void delegate_task(std::function<bool(Bucket)> todo, std::vector<Bucket>&& buckets_list);

    static ThreadPool* threadpool();

private:

    void process_tasks();
};

#endif //THREAD_POOL_HPP_INCLUDED