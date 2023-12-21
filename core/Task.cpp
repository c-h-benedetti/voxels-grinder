#include "Task.hpp"
#include "ThreadsPool.hpp"

int Task::run(Bucket b) {
    throw unsupported_operation("No available implementation for independant run.");
}

int Task::run(const Data* a, Bucket b) {
    throw unsupported_operation("No available implementation for Data base type.");
}


int Task::run(const VoxelsCanvas* a, Bucket b) {
    throw unsupported_operation("No available implementation for type: VoxelsCanvas.");
}


int Task::run(const GeometryCanvas* a, Bucket b) {
    throw unsupported_operation("No available implementation for type: GeometryCanvas.");
}


int Task::launch() {
    int status = 0;
    ThreadsPool* tp = ThreadsPool::threads_pool();

    status = this->preprocess();
    if (status != 0) { return status; }

    status = this->make_buckets();
    if (status != 0) { return status; }

    tp->delegate_task(this, this->buckets);
    status = tp->success();
    if (status != 0) { return status; }

    return this->postprocess();
}