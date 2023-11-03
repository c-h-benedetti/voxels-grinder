#ifndef MIN_MAX_CANVAS_STATS_HPP_INCLUDED
#define MIN_MAX_CANVAS_STATS_HPP_INCLUDED

#include <utility>
#include <vector>
#include "Task.hpp"
#include "Processor.hpp"

class MinMaxCanvas : public Processor {

    Data* target;
    std::vector<std::pair<float, float>> min_and_max;

private:

    struct MinMaxTask : public Task {
        std::vector<std::pair<float, float>> min_and_max;
        Data* target=nullptr;
        size_t channel=0;
        size_t frame=0;

        ~MinMaxTask() = default;

        MinMaxTask() = delete;
        MinMaxTask(Data* t, size_t c=0, size_t f=0);

        int run(const MaskCanvas*, Bucket b) override;
        int run(const LabeledCanvas*, Bucket b) override;
        int run(const VoxelsCanvasU8*, Bucket b) override;
        int run(const VoxelsCanvasU16*, Bucket b) override;
        int run(const VoxelsCanvasFloat*, Bucket b) override;
        int run(const VoxelsCanvasTriplet*, Bucket b) override;

        int execute(Bucket b) override;
    };

public:

    void process_min_max(size_t channel=0, size_t frame=0);
    bool min_max_available(size_t channel=0, size_t frame=0);
    const std::vector<std::pair<float ,float>>& get_min_max_by_slice(size_t channel=0, size_t frame=0);
    std::pair<float ,float> get_canvas_min_max(size_t channel=0, size_t frame=0);

    inline bool is_streamable() const override { return true; }

    MinMaxCanvas() = delete;
    MinMaxCanvas(Data* t);
};

#endif // MIN_MAX_CANVAS_STATS_HPP_INCLUDED