#include <limits>
#include "CanvasStatistics.hpp"
#include "NSlices.hpp"

Statistics::Statistics() : 
    min(std::numeric_limits<float>::infinity()), 
    max(-std::numeric_limits<float>::infinity()) {

}

int CanvasStatistics::ProcessStatsTask::run(const MaskCanvas* vc, Bucket b) {
    // Acquire reference to the output object.
    Statistics& s = this->stats[b.slices.first];

    // Update minimum and maximum
    return vc->dtype();
}

int CanvasStatistics::ProcessStatsTask::run(const LabeledCanvas* vc, Bucket b) {
    return vc->dtype();
}

int CanvasStatistics::ProcessStatsTask::run(const VoxelsCanvasU8* vc, Bucket b) {
    return vc->dtype();
}

int CanvasStatistics::ProcessStatsTask::run(const VoxelsCanvasU16* vc, Bucket b) {
    return vc->dtype();
}

int CanvasStatistics::ProcessStatsTask::run(const VoxelsCanvasFloat* vc, Bucket b) {

    return vc->dtype();
}

int CanvasStatistics::ProcessStatsTask::execute(Bucket b) {
    return this->target->run(this, b);
}


CanvasStatistics::ProcessStatsTask::ProcessStatsTask(Data* t) : target(t) {
    NSlices ns(target);
    ns.execute(Bucket()); // If not a VoxelCanvas, will throw an exception.
    size_t nSlices = ns.nSlices;
    this->stats = std::vector<Statistics>(nSlices);
}


// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=


std::vector<Bucket> CanvasStatistics::make_buckets() {

}

void CanvasStatistics::set_target(Data* t) {

}

void CanvasStatistics::process_statistics(size_t channel, size_t frame) {

}

bool CanvasStatistics::stats_available(size_t channel, size_t frame) {

}

const std::vector<Statistics>& CanvasStatistics::get_stats_by_slice(size_t channel, size_t frame) {

}

Statistics CanvasStatistics::get_canvas_stats(size_t channel, size_t frame) {

}

CanvasStatistics::CanvasStatistics(Data* t) {

}


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