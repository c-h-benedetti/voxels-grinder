#include "NSlices.hpp"


NSlicesTask::NSlicesTask(Data* t): target(t) {

}

int NSlicesTask::run(const MaskCanvas*, Bucket b) {
    this->nSlices = vc->get_global_dimensions().nSlices();
    return vc->dtype();
}

int NSlicesTask::run(const LabeledCanvas*, Bucket b) {
    this->nSlices = vc->get_global_dimensions().nSlices();
    return vc->dtype();
}

int NSlicesTask::run(const VoxelsCanvasU8*, Bucket b) {
    this->nSlices = vc->get_global_dimensions().nSlices();
    return vc->dtype();
}

int NSlicesTask::run(const VoxelsCanvasU16*, Bucket b) {
    this->nSlices = vc->get_global_dimensions().nSlices();
    return vc->dtype();
}

int NSlicesTask::run(const VoxelsCanvasFloat*, Bucket b) {
    this->nSlices = vc->get_global_dimensions().nSlices();
    return vc->dtype();
}

int NSlicesTask::run(const VoxelsCanvasTriplet*, Bucket b) {
    this->nSlices = vc->get_global_dimensions().nSlices();
    return vc->dtype();
}

int NSlicesTask::execute(Bucket b) {
    return this->target->run(this, b);
}