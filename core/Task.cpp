#include "Task.hpp"

int Task::run(const Data* a, Bucket b) {
    throw unsupported_operation("No available implementation for Data base type.");
}


int Task::run(const VoxelsCanvas* a, Bucket b) {
    throw unsupported_operation("No available implementation for type: VoxelsCanvas.");
}


int Task::run(const GeometryCanvas* a, Bucket b) {
    throw unsupported_operation("No available implementation for type: GeometryCanvas.");
}
