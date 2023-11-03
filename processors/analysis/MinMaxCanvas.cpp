#include <limits>
#include "MinMaxCanvas.hpp"
#include "NSlices.hpp"

MinMaxCanvas::MinMaxTask::MinMaxTask(Data* t, size_t c, size_t f) : target(t), channel(c), frame(f) {
    NSlices ns(target);
    ns.execute(Bucket());
    size_t nSlices = ns.nSlices;
    this->min_and_max = std::vector<std::pair<float, float>>(nSlices, {
        std::numeric_limits<float>::infinity(),
        -std::numeric_limits<float>::infinity()
    });
}

int MinMaxCanvas::MinMaxTask::run(const MaskCanvas*, Bucket b) {

}

int MinMaxCanvas::MinMaxTask::run(const LabeledCanvas*, Bucket b) {

}

int MinMaxCanvas::MinMaxTask::run(const VoxelsCanvasU8*, Bucket b) {

}

int MinMaxCanvas::MinMaxTask::run(const VoxelsCanvasU16*, Bucket b) {

}

int MinMaxCanvas::MinMaxTask::run(const VoxelsCanvasFloat*, Bucket b) {

}

int MinMaxCanvas::MinMaxTask::run(const VoxelsCanvasTriplet*, Bucket b) {

}

int MinMaxCanvas::MinMaxTask::execute(Bucket b) {

}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=


MinMaxCanvas::MinMaxCanvas(Data* t) : target(t) {}

void MinMaxCanvas::process_min_max(size_t channel, size_t frame) {
    MinMaxTask(target, channel, frame);
}

bool MinMaxCanvas::min_max_available(size_t channel, size_t frame) {

}

const std::vector<std::pair<float ,float>>& MinMaxCanvas::get_min_max_by_slice(size_t channel, size_t frame) {

}

std::pair<float ,float> MinMaxCanvas::get_canvas_min_max(size_t channel, size_t frame) {

}
