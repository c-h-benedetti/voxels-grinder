#include "CanvasDimensions.hpp"


CanvasDimensionsTask::CanvasDimensionsTask(Data* t): target(t) {}

template <typename T>
Bucket getDimensions(T* data) {
    return data->get_global_dimensions();
}

int CanvasDimensionsTask::run(const MaskCanvas* vc, Bucket b) {
    this->dimensions = getDimensions<MaskCanvas>(vc);
    return vc->dtype();
}

int CanvasDimensionsTask::run(const LabeledCanvas* vc, Bucket b) {
    this->dimensions = getDimensions<LabeledCanvas>(vc);
    return vc->dtype();
}

int CanvasDimensionsTask::run(const VoxelsCanvasU8* vc, Bucket b) {
    this->dimensions = getDimensions<VoxelsCanvasU8>(vc);
    return vc->dtype();
}

int CanvasDimensionsTask::run(const VoxelsCanvasU16* vc, Bucket b) {
    this->dimensions = getDimensions<VoxelsCanvasU16>(vc);
    return vc->dtype();
}

int CanvasDimensionsTask::run(const VoxelsCanvasFloat* vc, Bucket b) {
    this->dimensions = getDimensions<VoxelsCanvasFloat>(vc);
    return vc->dtype();
}

int CanvasDimensionsTask::run(const VoxelsCanvasTriplet* vc, Bucket b) {
    this->dimensions = getDimensions<VoxelsCanvasTriplet>(vc);
    return vc->dtype();
}

int CanvasDimensionsTask::execute(Bucket b) {
    return this->target->run(this, b);
}