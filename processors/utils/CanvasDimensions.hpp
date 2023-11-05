#ifndef TASK_READ_CANVAS_DIMENSIONS_HPP_INCLUDED
#define TASK_READ_CANVAS_DIMENSIONS_HPP_INCLUDED

#include "Task.hpp"

struct CanvasDimensionsTask : public Task {
    Data* target;
    Bucket dimensions;

    CanvasDimensionsTask() = delete;
    CanvasDimensionsTask(Data* t);

    ~CanvasDimensionsTask() = default;

    int run(const MaskCanvas*, Bucket b) override;
    int run(const LabeledCanvas*, Bucket b) override;
    int run(const VoxelsCanvasU8*, Bucket b) override;
    int run(const VoxelsCanvasU16*, Bucket b) override;
    int run(const VoxelsCanvasFloat*, Bucket b) override;
    int run(const VoxelsCanvasTriplet*, Bucket b) override;

    int execute(Bucket b) override;
};

#endif // TASK_READ_CANVAS_DIMENSIONS_HPP_INCLUDED