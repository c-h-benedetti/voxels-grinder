#ifndef CANVAS_STATISTICS_HPP_INCLUDED
#define CANVAS_STATISTICS_HPP_INCLUDED

#include <vector>
#include <utility>

#include "containers/VoxelsCanvas.hpp"
#include "core/Task.hpp"
#include "core/Processor.hpp"


struct Statistics {
    
    std::vector<uint32_t> histogram;
    float min, max;
    float mean, stddev;
    float q1, median, q3;
    float binSize;

    Statistics();
};


class CanvasStatistics : public Processor {
    
    /// Canvas from which we want to process the statistics.
    VoxelsCanvas* target;
    /// We make the statistics of the current stack. One index in the vector is the index of a slice.
    std::vector<std::vector<Statistics>> statistics;
    /// Number of bins in the histogram
    size_t nBins;
    /// Buckets used to multithread the extraction of statistics
    std::vector<Bucket> buckets;

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
        std::vector<std::vector<Statistics>>& statistics;
        
        /// The only implementation we need is for a VoxelsCanvas.
        int run(const VoxelsCanvas* vc, Bucket b) override;

        int execute(Bucket b) override;

        MinMaxSliceTask() = delete;
        MinMaxSliceTask(VoxelsCanvas* target, Bucket d, std::vector<std::vector<Statistics>>& s);
    };

    struct HistogramSliceTask : public Task {
        
        VoxelsCanvas* target; // Canvas from which we have to extract histograms
        std::vector<std::vector<Statistics>>& statistics; // Statistics containers.
        std::pair<float, float> extremas; // Start and end points of all histograms.
        size_t nBins; // Number of bins (slots) in the histogram array
        float binSize; // Size of a bin (number of values covered by a slot)

        int run(const VoxelsCanvas*, Bucket b) override;

        int execute(Bucket b) override;

        HistogramSliceTask() = delete;
        HistogramSliceTask(
            Data* t, 
            std::vector<Statistics>& s,
            size_t c, 
            size_t f, 
            std::pair<float, float> e, 
            size_t nb, 
            size_t bn
        );
    };

    std::vector<Bucket> make_buckets(size_t frame);
    void set_target(Data* t);

public:

    inline bool is_streamable() const override { return true; }

    void process_statistics();
    inline const std::vector<std::vector<Statistics>>& get_statistics() { return this->statistics; }

    CanvasStatistics() = delete;
    CanvasStatistics(Data* t, size_t nb=256);
};

#endif // CANVAS_STATISTICS_HPP_INCLUDED

/** NOTES: Étapes de la construction des statistiques
 *
 *  1. Avant de pouvoir faire l'histogramme, on doit avoir un nombre de bins et une bin size.
 *     Pour ce faire, nous avons besoin du minimum et du maximum du canvas.
 *     C'est la première task qui va être exécutée: sur chaque slice, on va chercher le minimum et le maximum.
 *     On récupère ces valeurs dans des objets Statistics qu'on a créés au préalable.
 * 
 * 2. Une fois cela accompli, on peut calculer le nombre de bins et la taille de bins de notre histogramme.
 *
**/
