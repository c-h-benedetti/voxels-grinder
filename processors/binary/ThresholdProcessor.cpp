#include <stdexcept>
#include <limits>
#include <iostream>

#include "ThresholdProcessor.hpp"
#include "core/ThreadsPool.hpp"
#include "containers/VoxelsCanvas.hpp"


Thresholder::Thresholder(Data* d, method m, bool by_slice) : Processor(), split_slices(by_slice) {
    if(!this->set_target(d)) {
        throw std::invalid_argument("[Thresholder]: The provided object couldn't be casted to a VoxelsCanvas.");
    }

    const Bucket& b = target->get_dimensions();
    bounds.resize(b.get_canvas_slices());

    for (size_t i = 0 ; i < b.get_canvas_slices() ; i++) {
        bounds[2*i+0].first = std::numeric_limits<float>::min();
        bounds[2*i+1].second = std::numeric_limits<float>::max();
    }

    this->set_threshold_method(m);
}


void Thresholder::process_bounds() {
    if (t_method == method::MANUAL) { return; }
}

/**
 * Sets the data on which we are going to work.
 *
 * @return 'true' if the target was successfuly modified, 'false' otherwise.
 */
bool Thresholder::set_target(Data* d) {
    // Reset current state anyway
    this->output = nullptr;
    this->target = nullptr;

    // If we don't have a VoxelsCanvas, we abort right away.
    VoxelsCanvas* vc = dynamic_cast<VoxelsCanvas*>(d);
    if (vc == nullptr) {
        return false;
    }

    this->target = vc;
    return true;
}


void Thresholder::apply_threshold() {

}


std::pair<float, float> Thresholder::get_bounds(size_t slice) {
    if (split_slices) { return bounds[slice]; }
    return bounds[0];
}


void Thresholder::set_bounds(float lower, float upper, size_t slice) {
    
}
