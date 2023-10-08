#include <stdexcept>
#include <limits>
#include "ThresholdTask.hpp"
#include "ThreadsPool.hpp"


ThresholderTask::ThresholderTask(Data* d, method m, bool by_slice) : Task("Threshold"), split_slices(by_slice) {
    if(!this->set_target(d)) {
        throw std::invalid_argument("[ThresholdTask]: The provided object couldn't be casted to a VoxelsCanvas.");
    }

    bounds.resize(2*target->getNSlices());

    for (size_t i = 0 ; i < target->getNSlices()) {
        bounds[2*i+0] = std::numeric_limits<float>::min();
        bounds[2*i+1] = std::numeric_limits<float>::max();
    }

    this->set_method(m);
}


void ThresholdTask::process_bounds() {
    if (t_method == method::MANUAL) { return; }
    switch (t_method) {
        case(method::OTSU):
            this->estimate_otsu();
            break;
        case(method::YEN):
            this->estimate_yen();
            break;
        case(method::MEAN):
            this->estimate_mean();
            break;
        case(method::INTERMODES):
            this->estimate_intermodes();
            break;
        case(method::KAPUR):
            this->estimate_kapur();
            break;
        case(method::SHANBHAG):
            this->estimate_shanbhag();
            break;
        case(method::LI):
            this->estimate_li();
            break;
        case(method::ROSIN):
            this->estimate_rosin();
            break;
        case(method::ADAPTIVE):
            this->estimate_adaptive();
            break;
        case(method::SAUVOLA):
            this->estimate_sauvola();
            break;
        case(method::PHANSALKAR):
            this->estimate_phansalkar();
            break;
        case(method::TRIANGLE):
            this->estimate_triangle();
            break;
        case(method::MEDIAN):
            this->estimate_median();
            break;
        case(method::HYSTERESIS):
            this->estimate_hysteresis();
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
    
    // Try to cast input into VoxelsCanvas
    VoxelsCanvas* vc = dynamic_cast<VoxelsCanvas*>(d);

    // If we don't have a VoxelsCanvas, we abort right away.
    if (vc == nullptr) {
        return false;
    }

    this->target = vc;
    this->output = std::make_unique<VoxelsCanvas>(target->getCanvasShape());
    return true;
}


void ThresholdTask::apply_threshold() {

}


float ThresholdTask::get_lower_bound(size_t slice) {
    if (split_slices) { return bounds[2*slice]; }
    return bounds[0];
}


float ThresholdTask::get_upper_bound(size_t slice) {
    if (split_slices) { return bounds[2*slice+1]; }
    return bounds[1];
}


void ThresholdTask::set_target(VoxelsCanvas* vc) {
    this->target = vc;
    if (target == nullptr) { return; }
    output = std::unique_ptr<VoxelsCanvas>(target->alike());
}


void ThresholdTask::set_bounds(float lower, float upper, size_t slice) {
    
    if (t_method != method::MANUAL) {
        std::cerr << "[ThresholdTask] Bounds can only be overwritten in manual thresholding mode." << std::endl;
        return; 
    }

    size_t p0=0, p1=1;

    if (split_slices) {
        if (slice >= target->getNSlices()) { 
            std::cerr << "[ThresholdTask] The provided index is beyond the number of slices in the canvas." << std::endl;
            return; 
        }
        p0 = 2*slice;
        p1 = 2*slice+1;
    }

    bounds[p0] = lower;
    bounds[p1] = (lower > upper) ? lower : upper;
}

// ============== THRESHOLDING ALGORITHMS ============== 

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
