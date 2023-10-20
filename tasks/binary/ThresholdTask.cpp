#include <stdexcept>
#include <limits>
#include <iostream>
#include "ThresholdTask.hpp"
#include "ThreadsPool.hpp"


ThresholdTask::ThresholdTask(Data* d, method m, bool by_slice) : Task(), split_slices(by_slice) {
    if(!this->set_target(d)) {
        throw std::invalid_argument("[ThresholdTask]: The provided object couldn't be casted to a VoxelsCanvas.");
    }

    const Bucket& b = target->get_global_size();
    bounds.resize(2*b.nSlices());

    for (size_t i = 0 ; i < b.nSlices() ; i++) {
        bounds[2*i+0] = std::numeric_limits<float>::min();
        bounds[2*i+1] = std::numeric_limits<float>::max();
    }

    this->set_threshold_method(m);
}


void ThresholdTask::process_bounds() {
    if (t_method == method::MANUAL) { return; }
    switch (t_method) {
        case(method::OTSU):
            // this->estimate_otsu();
            break;
        case(method::YEN):
            // this->estimate_yen();
            break;
        case(method::MEAN):
            // this->estimate_mean();
            break;
        case(method::INTERMODES):
            // this->estimate_intermodes();
            break;
        case(method::KAPUR):
            // this->estimate_kapur();
            break;
        case(method::SHANBHAG):
            // this->estimate_shanbhag();
            break;
        case(method::LI):
            // this->estimate_li();
            break;
        case(method::ROSIN):
            // this->estimate_rosin();
            break;
        case(method::ADAPTIVE):
            // this->estimate_adaptive();
            break;
        case(method::SAUVOLA):
            // this->estimate_sauvola();
            break;
        case(method::PHANSALKAR):
            // this->estimate_phansalkar();
            break;
        case(method::TRIANGLE):
            // this->estimate_triangle();
            break;
        case(method::MEDIAN):
            // this->estimate_median();
            break;
        case(method::HYSTERESIS):
            // this->estimate_hysteresis();
            break;
        default:
            std::cerr << "[ThresholdTask]: Requested method is not handled or doesn't exist." << std::endl;
    };
}

/**
 * Sets the data on which we are going to work.
 *
 * @return 'true' if the target was successfuly modified, 'false' otherwise.
 */
bool ThresholdTask::set_target(Data* d) {
    // Reset current state anyway
    this->output = nullptr;
    this->target = nullptr;

    // If we don't have a VoxelsCanvas, we abort right away.
    if (!Data::is_voxels_canvas(d->dtype())) {
        return false;
    }

    this->target = d;
    return true;
}


void ThresholdTask::apply_threshold() {

}


std::pair<float, float> ThresholdTask::get_bounds(size_t slice) {
    if (split_slices) { return bounds[slice]; }
    return bounds[0];
}


void ThresholdTask::set_bounds(float lower, float upper, size_t slice) {
    
    if (t_method != method::MANUAL) {
        std::cerr << "[ThresholdTask] Bounds can only be overwritten in manual thresholding mode." << std::endl;
        return; 
    }

    size_t p=0;

    if (split_slices) {
        Bucket b = target->get_global_size();
        if (slice >= b.nSlices()) { 
            std::cerr << "[ThresholdTask] The provided index is beyond the number of slices in the canvas." << std::endl;
            return; 
        }
        p = slice;
    }

    bounds[p] = std::pair<float, float>(lower, (lower > upper) ? lower : upper);
}

// ========================= THRESHOLDING ALGORITHMS ========================= 

bool ThresholdTask::estimate_otsu(Bucket b) {

}

bool ThresholdTask::estimate_yen(Bucket b) {

}

bool ThresholdTask::estimate_mean(Bucket b) {

}

bool ThresholdTask::estimate_intermodes(Bucket b) {

}

bool ThresholdTask::estimate_kapur(Bucket b) {

}

bool ThresholdTask::estimate_shanbhag(Bucket b) {

}

bool ThresholdTask::estimate_li(Bucket b) {

}

bool ThresholdTask::estimate_rosin(Bucket b) {

}

bool ThresholdTask::estimate_adaptive(Bucket b) {

}

bool ThresholdTask::estimate_sauvola(Bucket b) {

}

bool ThresholdTask::estimate_phansalkar(Bucket b) {

}

bool ThresholdTask::estimate_triangle(Bucket b) {

}

bool ThresholdTask::estimate_median(Bucket b) {

}

bool ThresholdTask::estimate_hysteresis(Bucket b) {

}
