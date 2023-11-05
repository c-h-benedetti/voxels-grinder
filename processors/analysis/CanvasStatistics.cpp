#include <limits>
#include "CanvasStatistics.hpp"
#include "CanvasDimensions.hpp"

// =========== Implementation of the [Statistics] class ===================

Statistics::Statistics() : 
    min(std::numeric_limits<float>::infinity()), 
    max(-std::numeric_limits<float>::infinity()) {

}


// ========== Implementation of the [MinMaxSliceTask] class ===============


int MinMaxSliceTask::run(const MaskCanvas*, Bucket b) {

}

int MinMaxSliceTask::run(const LabeledCanvas*, Bucket b) {

}

int MinMaxSliceTask::run(const VoxelsCanvasU8*, Bucket b) {

}

int MinMaxSliceTask::run(const VoxelsCanvasU16*, Bucket b) {

}

int MinMaxSliceTask::run(const VoxelsCanvasFloat*, Bucket b) {

}

int MinMaxSliceTask::execute(Bucket b) {
    return this->target->run(this, b);
}

MinMaxSliceTask::MinMaxSliceTask(Data* t, Bucket d, std::vector<Statistics>& s, size_t c, size_t f): 
    target(t), 
    nSlices(n), 
    dimensions(d),
    channel(c),
    frame(f) {

}


// ========== Implementation of the [BinningPropertiesTask] class ===============


size_t pgcd(size_t a, size_t b) {
    size_t t = 0;
    while (b != 0) {
        t = b;
        b = a%b;
        a = t;
    }
    return a;
}

int BinningPropertiesTask::run(const MaskCanvas*, Bucket b) {
    float diff = this->extremas.second - this->extremas.first;
    this->binSize = 1.0f;
    this->nBins = static_cast<size_t>(diff);
}

int BinningPropertiesTask::run(const LabeledCanvas*, Bucket b) {
    float diff = this->extremas.second - this->extremas.first;
    if (diff > 65536.0f) {
        this->nBins = pgcd(65536, static_cast<size_t>(diff));
        this->binSize = diff / static_cast<float>(this->nBins);
    }
    else {
        this->nBins = static_cast<size_t>(diff);
        this->binSize = 1.0f;
    }
}

int BinningPropertiesTask::run(const VoxelsCanvasU8*, Bucket b) {
    float diff = this->extremas.second - this->extremas.first;
    this->binSize = 1.0f;
    this->nBins = static_cast<size_t>(diff);
}

int BinningPropertiesTask::run(const VoxelsCanvasU16*, Bucket b) {
    float diff = this->extremas.second - this->extremas.first;
    this->binSize = 1.0f;
    this->nBins = static_cast<size_t>(diff);
}

int BinningPropertiesTask::run(const VoxelsCanvasFloat*, Bucket b) {
    float diff = this->extremas.second - this->extremas.first;
    if (diff > 65536.0f) {
        this->nBins = pgcd(65536, static_cast<size_t>(diff));
        this->binSize = diff / static_cast<float>(this->nBins);
    }
    else {
        this->nBins = static_cast<size_t>(diff);
        this->binSize = 1.0f;
    }
}

int BinningPropertiesTask::execute(Bucket b) {
    return this->target->run(this, b);
}

BinningPropertiesTask::BinningPropertiesTask(Data* t, std::pair<float, float> e) : target(t), extremas(e) {}


// ========== Implementation of the [HistogramSliceTask] class ===============


int HistogramSliceTask::run(const MaskCanvas* vc Bucket b) {
    size_t current_slice = b.slices.first;
    size_t current_channel = b.channels.first;
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

int HistogramSliceTask::run(const LabeledCanvas* vc, Bucket b) {

}

int HistogramSliceTask::run(const VoxelsCanvasU8* vc, Bucket b) {

}

int HistogramSliceTask::run(const VoxelsCanvasU16* vc, Bucket b) {

}

int HistogramSliceTask::run(const VoxelsCanvasFloat* vc, Bucket b) {

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


std::vector<Bucket> CanvasStatistics::make_buckets(size_t channel, size_t frame) {
    size_t nSlices = this->dimensions.nSlices();
    std::vector<Bucket> buckets;
    buckets.reserve(nSlices);

    // We create buckets to delegate a slice to each thread.
    for (size_t i = 0 ; i < this->dimensions.nSlices() ; i++) {
        Bucket b = this->dimensions;
        b.channels = {channel, channel+1};
        b.frames = {frame, frame+1};
        b.slices = {i, i+1};
        buckets.push_back(b);
    }
    return buckets;
}

void CanvasStatistics::set_target(Data* t) {
    if (!Data::is_voxels_canvas(t->dtype())) { throw Ithrow std::invalid_argument("Is not a VoxelsCanvas."); }
    this->target = t;
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

const std::vector<Statistics>& CanvasStatistics::get_stats_by_slice() {
    return this->statistics;
}

Statistics CanvasStatistics::get_canvas_stats() {

}

CanvasStatistics::CanvasStatistics(Data* t) {
    this->set_target(t); // If not a VoxelCanvas, will throw an exception.
    CanvasDimensions cd(target);
    cd.execute(Bucket()); // If not a VoxelCanvas, will throw an exception.
    this->dimensions = cd.dimensions;
    this->stats = std::vector<Statistics>(this->dimensions.nSlices());
}


// ================================  USE CASE  ==================================


void usage_example() {
    size_t nChannels = 3;
    VoxelsCanvasU16 vc16(1080, 1920, 15, nChannels, 1);
    CanvasStatistics cs(&vc16);

    for (size_t c = 0 ; c < nChannels ; c++) {
        cs.process_statistics(c);
    }

    for (size_t c = 0 ; c < nChannels ; c++) {
        if (!cs.stats_available(c)) { continue; }
        const std::vector<Statistics>& stats = cs.get_stats_by_slice(0);
        for (const Statistics& s : stats) {
            std::cout << "(" << c << ") Min: " << s.min << ", Max: " << s.max << std::endl;
        }
    }
}