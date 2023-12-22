#ifndef CANVAS_STATISTICS_HPP_INCLUDED
#define CANVAS_STATISTICS_HPP_INCLUDED

#include <vector>
#include <utility>

#include "containers/VoxelsCanvas.hpp"
#include "core/Task.hpp"
#include "core/Processor.hpp"

/// Contains a set of statistics.
struct Statistics {
    
    std::vector<uint32_t> histogram;
    float min, max;
    float mean, stddev;
    float q1, median, q3;

    Statistics();
};

/// Collection of Statistics object to represent each slice of a stack.
struct StackStatistics {
    std::vector<Statistics> slices_statistics;
    float min, max;
    float binSize;

    StackStatistics() = default;
    StackStatistics(size_t s);
};


class CanvasStatisticsProcessor : public Processor {
    
    /// Canvas from which we want to process the statistics.
    VoxelsCanvas* target;
    /// We make the statistics of the current stack. We make statistics for every each slice of each frame.
    std::vector<StackStatistics> frames_statistics;
    /// Number of bins in the histogram
    size_t nBins;

private:

    /**
     * Task searching for the minimal and the maximal values over a given slice.
     * It is mainly intended for regular images as some binning occurs in the histogram.
     * To get a representation of some labels, this is not the most suitable.
     */
    struct MinMaxSliceTask : public Task {
        /// Canvas of which we want to process the statistics.
        VoxelsCanvas* target;
        /// Statistics of every individual slice and every frame.
        std::vector<StackStatistics>& frames_statistics;
        
        /// The only implementation we need is for a VoxelsCanvas.
        int run(const VoxelsCanvas* vc, Bucket b) override;
        int execute(Bucket b) override;
        int make_buckets() override;
        int postprocess() override;

        MinMaxSliceTask() = delete;
        MinMaxSliceTask(VoxelsCanvas* target, std::vector<StackStatistics>& s);
    };

    /**
     * Task building the histogram for a whole canvas.
     * The number of bins is the same for each histogram (each slice from each frame).
     * Both the bin size and the extremums are common to every slices of a given frame.
     * The statistics summarized for the whole canvas are not processed here.
     */
    struct HistogramSliceTask : public Task {
        /// Canvas from which we have to extract histograms
        VoxelsCanvas* target;
        /// Statistics containers.
        std::vector<StackStatistics>& frames_statistics;
        /// Number of bins (slots) in the histogram array
        size_t nBins;
        /// Size of a bin (number of values covered by a slot)
        float binSize;

        int run(const VoxelsCanvas*, Bucket b) override;
        int execute(Bucket b) override;
        int make_buckets() override;
        int preprocess() override;

        HistogramSliceTask() = delete;
        HistogramSliceTask(
            VoxelsCanvas* t, 
            std::vector<StackStatistics>& s,
            size_t nb
        );
    };

    struct ProcessStatisticsTask : public Task {
        /// Statistics of every individual slice and every frame.
        std::vector<StackStatistics>& frames_statistics;
        /// Canvas from which we have to extract histograms
        VoxelsCanvas* target;
        
        /// The only implementation we need is for a VoxelsCanvas.
        int run(Bucket b) override;
        int execute(Bucket b) override;
        int make_buckets() override;

        ProcessStatisticsTask() = delete;
        ProcessStatisticsTask(VoxelsCanvas* t, std::vector<StackStatistics>& s);
    };

    void set_target(Data* t);

public:

    inline bool is_streamable() const override { return true; }

    void process_statistics();
    inline const std::vector<StackStatistics>& get_statistics() { return this->frames_statistics; }

    CanvasStatisticsProcessor() = delete;
    CanvasStatisticsProcessor(Data* t, size_t nb=512);
};

#endif // CANVAS_STATISTICS_HPP_INCLUDED

/** TODO:
 *
 * - [ ] Modify this processor to take into account a possible selection on the canvas.
 * - [ ] Better way to determine the number of bins?
 */