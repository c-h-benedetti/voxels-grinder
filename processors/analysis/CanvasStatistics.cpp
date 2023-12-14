#include <limits>
#include "CanvasStatistics.hpp"
#include "processors/utils/CanvasDimensions.hpp"

// =========== Implementation of the [Statistics] class ===================

Statistics::Statistics() : 
    min(std::numeric_limits<float>::infinity()), 
    max(-std::numeric_limits<float>::infinity()) {

}

size_t pgcd(size_t a, size_t b) {
    size_t t = 0;
    while (b != 0) {
        t = b;
        b = a%b;
        a = t;
    }
    return a;
}


// ========== Implementation of the [HistogramSliceTask] class ===============


int HistogramSliceTask::run(const VoxelsCanvas* vc, Bucket b) {
    size_t current_slice = b.slices.first;
    size_t current_frame = b.frames.first;

    if (this->slices[current_slice].histogram.size() != this->nBins) {
        this->slices[current_slice].histogram = std::vector<uint32_t>(this->nBins, 0);
    }

    std::vector<uint32_t>& histogram = this->slices[current_slice].histogram;
    float diff = this->extremas.second - this->extremas.first;
    size_t index;
    
    for (size_t l = b.lines.first ; l < b.lines.second ; l++) {
        for (size_t c = b.columns.first ; c < b.columns.second ; c++) {
            index = static_cast<size_t>((vc->get(l, c) - this->extremas.first) / this->nBins);
            histogram[index]++;
        }
    }
}

int HistogramSliceTask::execute(Bucket b) {
    return this->target->run(this, b);
}

HistogramSliceTask::HistogramSliceTask(Data* t, std::vector<Statistics>& s, Bucket d, size_t c, size_t f, std::pair<float, float> e, size_t nb, size_t bn):
    target(t),
    slices(s),
    dimensions(d),
    channel(c),
    frames(f),
    extremas(e),
    nBins(nb),
    binSize(bn) {

}


// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=


std::vector<Bucket> CanvasStatistics::make_buckets(Bucket dimensions) {
    size_t nSlices = dimensions.nSlices();
    std::vector<std::vector<Bucket>> buckets;
    buckets.reserve(nSlices);

    // We create buckets to delegate a slice to each thread.
    for (size_t f = 0 ; f < dimensions.nFrames() ; f++){
        for (size_t s = 0 ; s < nSlices ; s++) {
            Bucket b = dimensions;
            b.frames = {frame, frame+1};
            b.slices = {i, i+1};
            buckets.push_back(b);
        }
    }

    return buckets;
}


void CanvasStatistics::process_statistics(size_t channel, size_t frame) {
    // 0. Making the list of buckets for the ThreadsPool
    std::vector<Bucket> buckets = this->make_buckets(channel, frame);
    // 1. Processing min and max of each slice.
    MinMaxSliceTask mmst(this->target, this->nSlices, this->statistics, channel, frame);
    mmst.launch(buckets);
    // 2. Histogram settings (# bins and bin size)
    float global_extremas = std::pair<float, float>(this->statistics[0].min, this->statistics[0].max);
    for (const Statistics& s : this->statistics) {
        if (s.min < global_extremas.first)  {global_extremas.first = s.min; }
        if (s.max > global_extremas.second) { global_extremas.second = s.max; }
    }
    BinningPropertiesTask bpt(this->target, global_extremas);
    bpt.execute(Bucket());
    // 3. Building each slice's histogram
}


void CanvasStatistics::set_target(Data* t) {
    this->target = dynamic_cast<VoxelsCanvas*>(t);
    if (this->target == nullptr) { throw std::invalid_argument("Is not a VoxelsCanvas."); }
}


CanvasStatistics::CanvasStatistics(Data* t, size_t nb): nBins(nb) {
    this->set_target(t); // If not a VoxelCanvas, will throw an exception.
    Bucket dimensions = this->target->get_global_dimensions();
    this->statistics = std::vector<std::vector<Statistics>>(
        this->dimensions.nFrames(), 
        std::vector<Statistics>(this->dimensions.nSlices())
    );
    this->buckets = this->make_buckets(dimensions);
}
