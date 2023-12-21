#include <limits>
#include <algorithm>
#include "CanvasStatisticsProcessor.hpp"

// =========== Implementation of the [Statistics] and [StackStatistics] class ===================

Statistics::Statistics() : 
    min(std::numeric_limits<float>::infinity()), 
    max(-std::numeric_limits<float>::infinity()) {}


StackStatistics::StackStatistics(size_t s): 
    min(std::numeric_limits<float>::infinity()), 
    max(-std::numeric_limits<float>::infinity()), 
    slices_statistics(s) {}


// ========== Implementation of the [MinMaxSliceTask] class ===============

// OK
CanvasStatisticsProcessor::MinMaxSliceTask::MinMaxSliceTask(VoxelsCanvas* t, std::vector<StackStatistics>& s) : target(t), frames_statistics(s) {}

// OK
int CanvasStatisticsProcessor::MinMaxSliceTask::run(const VoxelsCanvas* vc, Bucket b) {
    Statistics& buffer = this->frames_statistics[b.get_frames_range().first].slices_statistics[b.get_slices_range().first];
    Bucket::Iterator it = b.get_iterator();
    float val, t_min=buffer.min, t_max=buffer.max;

    while (it) {
        val = vc->at(it);
        if (val > t_max) { t_max = val; }
        if (val < t_min) { t_min = val; }
        it.next();
    }

    buffer.min = t_min;
    buffer.max = t_max;
    return 0;
}

// OK
int CanvasStatisticsProcessor::MinMaxSliceTask::execute(Bucket b) {
    return this->target->run(this, b);
}


// OK
int CanvasStatisticsProcessor::MinMaxSliceTask::make_buckets() {
    Bucket dimensions = this->target->get_dimensions();
    size_t nSlices = dimensions.get_canvas_slices();
    size_t nFrames = dimensions.get_canvas_frames();
    size_t total   = nSlices * nFrames;
    this->buckets.clear();
    this->buckets.reserve(total);

    // We create buckets to delegate a slice to each thread.
    for (size_t f = 0 ; f < nFrames ; f++){
        for (size_t s = 0 ; s < nSlices ; s++) {
            Bucket b(dimensions);
            b.set_local_frames({f, f+1});
            b.set_local_slices({s, s+1});
            this->buckets.push_back(b);
        }
    }

    return 0;
}

// OK
int CanvasStatisticsProcessor::MinMaxSliceTask::postprocess() {
    float val, t_min, t_max;
    for (size_t f = 0 ; f < this->frames_statistics.size() ; f++) {
        t_min = this->frames_statistics[f].min;
        t_max = this->frames_statistics[f].max;
        for (size_t s = 0 ; s < this->frames_statistics[f].slices_statistics.size() ; s++) {
            val = this->frames_statistics[f].slices_statistics[s].min;
            if (val < t_min) { t_min = val; }
            val = this->frames_statistics[f].slices_statistics[s].max;
            if (val > t_max) { t_max = val; }
        }
        this->frames_statistics[f].min = t_min;
        this->frames_statistics[f].max = t_max;
    }

    return 0;
}


// ========== Implementation of the [HistogramSliceTask] class ===============


int CanvasStatisticsProcessor::HistogramSliceTask::run(const VoxelsCanvas* vc, Bucket b) {
    size_t s = b.get_slices_range().first, f = b.get_frames_range().first;
    size_t t_min = this->frames_statistics[f].min, t_max = this->frames_statistics[f].max;
    float diff = t_max - t_min;
    Bucket::Iterator it = b.get_iterator();
    std::vector<uint32_t>& histo = this->frames_statistics[f].slices_statistics[s].histogram;

    while (it) {
        histo[static_cast<size_t>((vc->at(it) - t_min) / diff)]++;
        it.next();
    }

    return 0;
}


int CanvasStatisticsProcessor::HistogramSliceTask::execute(Bucket b) {
    return this->target->run(this, b);
}


int CanvasStatisticsProcessor::HistogramSliceTask::make_buckets() {
    Bucket dimensions = this->target->get_dimensions();
    size_t nSlices = dimensions.get_canvas_slices();
    size_t nFrames = dimensions.get_canvas_frames();
    size_t total   = nSlices * nFrames;
    this->buckets.clear();
    this->buckets.reserve(total);

    // We create buckets to delegate a slice to each thread.
    for (size_t f = 0 ; f < nFrames ; f++){
        for (size_t s = 0 ; s < nSlices ; s++) {
            Bucket b(dimensions);
            b.set_local_frames({f, f+1});
            b.set_local_slices({s, s+1});
            this->buckets.push_back(b);
        }
    }

    return 0;
}


int CanvasStatisticsProcessor::HistogramSliceTask::preprocess() {
    for (size_t f = 0 ; f < this->frames_statistics.size() ; f++) {

        float t_max = this->frames_statistics[f].max;
        float t_min = this->frames_statistics[f].min;
        this->frames_statistics[f].binSize = (t_max - t_min) < 0.000001f ? 0.0f : static_cast<float>(this->nBins) / (t_max - t_min);

        for (size_t s = 0 ; s < this->frames_statistics[f].slices_statistics.size() ; s++) {
            this->frames_statistics[f].slices_statistics[s].histogram = std::vector<uint32_t>(this->nBins, 0);
        }
    }
    return 0;
}


CanvasStatisticsProcessor::HistogramSliceTask::HistogramSliceTask(
    VoxelsCanvas* t, 
    std::vector<StackStatistics>& s,
    size_t nb
) : target(t), 
    frames_statistics(s),
    nBins(nb) {}



// ========== Implementation of the [ProcessStatisticsTask] class ===============


int CanvasStatisticsProcessor::ProcessStatisticsTask::run(Bucket b) {
    size_t f = b.get_frames_range().first;
    size_t s = b.get_slices_range().first;
    Statistics& stats = this->frames_statistics[f].slices_statistics[s];
    size_t nVoxels = b.get_canvas_height() * b.get_canvas_width();
    float total_slice = static_cast<float>(nVoxels);

    // ---- Processing mean ----
    float accumulator = 0.0f;
    float coef = 0.0f, val = 0.0f;
    float min = this->frames_statistics[f].min;
    float shift = this->frames_statistics[f].binSize * 0.5f;

    for (size_t i = 0 ; i < stats.histogram.size() ; i++) {
        coef = static_cast<float>(stats.histogram[i]) / total_slice;
        val  = min + (this->frames_statistics[f].binSize + shift) * i;
        accumulator += val * coef;
    }

    stats.mean = accumulator;

    // ---- Standard deviation ----
    float std_dev_sq = 0.0f;
    float buffer = 0.0f;
    for (size_t i = 0 ; i < stats.histogram.size() ; i++) {
        val = min + (this->frames_statistics[f].binSize + shift) * i;
        buffer = val - accumulator;
        buffer *= buffer;
        buffer *= stats.histogram[i];
        std_dev_sq += buffer;
    }
    std_dev_sq /= (total_slice-1.0f);

    stats.stddev = sqrt(std_dev_sq);

    // ---- Cumulative histogram, Q1, Median, Q3 ----
    std::vector<uint32_t> cumulative(stats.histogram);
    for (size_t i = 1 ; i < cumulative.size() ; i++) {
        cumulative[i] += cumulative[i-1];
    }
    
    size_t voxels_25_p = nVoxels / 4;
    size_t voxels_50_p = nVoxels / 2;
    size_t voxels_75_p = 3 * voxels_25_p;

    auto it_25 = std::lower_bound(cumulative.begin(), cumulative.end(), voxels_25_p);
    auto it_50 = std::lower_bound(cumulative.begin(), cumulative.end(), voxels_50_p);
    auto it_75 = std::lower_bound(cumulative.begin(), cumulative.end(), voxels_75_p);

    std::ptrdiff_t index_25 = std::distance(cumulative.begin(), it_25);
    std::ptrdiff_t index_50 = std::distance(cumulative.begin(), it_50);
    std::ptrdiff_t index_75 = std::distance(cumulative.begin(), it_75);

    stats.q1 = min + (this->frames_statistics[f].binSize + shift) * index_25;
    stats.median = min + (this->frames_statistics[f].binSize + shift) * index_50;
    stats.q3 = min + (this->frames_statistics[f].binSize + shift) * index_75;
    
    return 0;
}


int CanvasStatisticsProcessor::ProcessStatisticsTask::execute(Bucket b) {
    return this->run(b);
}


int CanvasStatisticsProcessor::ProcessStatisticsTask::make_buckets() {
    Bucket dimensions = this->target->get_dimensions();
    size_t nSlices = dimensions.get_canvas_slices();
    size_t nFrames = dimensions.get_canvas_frames();
    size_t total   = nSlices * nFrames;
    this->buckets.clear();
    this->buckets.reserve(total);

    // We create buckets to delegate a slice to each thread.
    for (size_t f = 0 ; f < nFrames ; f++){
        for (size_t s = 0 ; s < nSlices ; s++) {
            Bucket b(dimensions);
            b.set_local_frames({f, f+1});
            b.set_local_slices({s, s+1});
            this->buckets.push_back(b);
        }
    }

    return 0;
}


CanvasStatisticsProcessor::ProcessStatisticsTask::ProcessStatisticsTask(VoxelsCanvas* t, std::vector<StackStatistics>& s) : target(t), frames_statistics(s) {}


// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=


void CanvasStatisticsProcessor::process_statistics() {
    // 1. Processing min and max of each slice.
    MinMaxSliceTask mmst(this->target, this->frames_statistics);
    mmst.launch();
    // 2. Building the histogram.
    HistogramSliceTask hst(this->target, this->frames_statistics, this->nBins);
    hst.launch();
}


// OK
void CanvasStatisticsProcessor::set_target(Data* t) {
    this->target = dynamic_cast<VoxelsCanvas*>(t);
    if (this->target == nullptr) { throw std::invalid_argument("Is not a VoxelsCanvas."); }
}

// OK
CanvasStatisticsProcessor::CanvasStatisticsProcessor(Data* t, size_t nb): nBins(nb) {
    this->set_target(t); // If not a VoxelCanvas, will throw an exception.
    Bucket dimensions = this->target->get_dimensions();

    this->frames_statistics = std::vector<StackStatistics>(
        dimensions.get_canvas_frames(), 
        StackStatistics(dimensions.get_canvas_slices())
    );
}
