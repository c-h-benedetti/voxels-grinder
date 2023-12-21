#include <vector>
#include <utility>
#include <random>

#include "containers/VoxelsCanvas.hpp"
#include "containers/GeometryCanvas.hpp"
#include "processors/analysis/CanvasStatisticsProcessor.hpp"
#include "gtest/gtest.h"


TEST(CanvasStatistics, Constructor) {
    RecordProperty("short", "Process min and max of canvas");

    VoxelsCanvas   vc1(1, 1, 1, 1);
    GeometryCanvas gc1;

    Data* d1 = nullptr;
    Data* d2 = (Data*)&gc1;
    Data* d3 = (Data*)&vc1;

    EXPECT_ANY_THROW(CanvasStatisticsProcessor csp(d1));
    EXPECT_ANY_THROW(CanvasStatisticsProcessor csp(d2));
    EXPECT_NO_THROW(CanvasStatisticsProcessor csp(d3));
}


TEST(CanvasStatistics, ProcessMinMax) {
    RecordProperty("short", "Process min and max of canvas");

    constexpr size_t width = 128;
    constexpr size_t height = 128;
    constexpr size_t slices = 8;
    constexpr size_t frames = 4;

    Bucket b1(width, height, slices, frames);
    VoxelsCanvas vc(b1);
    float current = 0.0f, start;
    std::vector<std::pair<float, float>> min_max_frames;

    for (size_t f = 0 ; f < frames ; f++) {
        for (size_t s = 0 ; s < slices ; s++) {
            start = current;
            for (size_t l = 0 ; l < height ; l++) {
                for (size_t c = 0 ; c < width ; c++) {
                    vc.set(c, l, s, f, current);
                    current += 1.0f;
                }
            }
            min_max_frames.push_back({start, current-1.0f});
        }
    }

    CanvasStatisticsProcessor csp((Data*)&vc);
    csp.process_statistics();
    const std::vector<StackStatistics>& stats = csp.get_statistics();
    size_t rank = 0;

    EXPECT_EQ(stats.size(), frames);

    for (size_t f = 0 ; f < stats.size() ; f++) {
        EXPECT_EQ(stats[f].slices_statistics.size(), slices);
        for (size_t s = 0 ; s < stats[f].slices_statistics.size() ; s++) {
            const Statistics& s_stats = stats[f].slices_statistics[s];
            EXPECT_EQ(min_max_frames[rank].first, s_stats.min);
            EXPECT_EQ(min_max_frames[rank].second, s_stats.max);
            rank++;
        }
    }
}


TEST(CanvasStatistics, BuildHistogram) {
    RecordProperty("short", "Process min and max of canvas");

    constexpr size_t width = 128;
    constexpr size_t height = 128;
    constexpr size_t slices = 8;
    constexpr size_t frames = 4;
    constexpr float start = 5.0f, end = 125.0f;

    Bucket b1(width, height, slices, frames);
    VoxelsCanvas vc(b1);
    Bucket::Iterator it = b1.get_iterator();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(start, end);

    while (it) {
        vc.set(it, dis(gen));
        it.next();
    }

    CanvasStatisticsProcessor csp((Data*)&vc, static_cast<size_t>(1+end-start));
    csp.process_statistics();

    const std::vector<StackStatistics>& stats = csp.get_statistics();

    // All the slices of a same frame have a histogram of the same size.
    for (size_t f = 0 ; f < frames ; f++) {
        size_t histoSize = stats[f].slices_statistics[0].histogram.size();
        for (size_t s = 0 ; s < slices ; s++) {
            EXPECT_EQ(histoSize, stats[f].slices_statistics[s].histogram.size());
        }
    }
}

/** TESTS:
 * 
 * >>> CanvasStatisticsProcessor
 * 
 * - [ ] Le total de l'histogramme pour chaque slice est égal à height*width.
 * - [ ] Gère le cas où le canvas est rempli avec la même valeur.
 * - [ ] Vérification des valeurs de calculs.
 * 
 */