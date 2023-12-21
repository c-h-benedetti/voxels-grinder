#ifndef VOXELS_CANVAS_HPP_INCLUDED
#define VOXELS_CANVAS_HPP_INCLUDED

#include <vector>
#include <filesystem>
#include <stdexcept>
#include "core/Data.hpp"
#include "core/Bucket.hpp"

class VoxelsCanvas : public Data {

    /// Actual data looaded from the disk in a 1D array.
    float* data = nullptr;
    /// Dimensions of the canvas.
    Bucket dimensions;

private:

    bool allocate_memory(Bucket b);

public:

    enum class DisplayFlags : uint64_t {
        image  = 0,
        mask   = 1 << 0,
        labels = 1 << 1
    };

    enum class padding_type {
        BACKGROUND, /// Pad with the value 0.
        DRIFT, /// Pad with the last value encountered.
        LOOP /// Acts like there is a mirror effect.
    };

    inline const float* get_data_segment() { return this->data; }
    inline Bucket get_dimensions() const { return this->dimensions; }
    int run(Task* v, Bucket b) override;

    ~VoxelsCanvas();

    VoxelsCanvas() = delete;
    VoxelsCanvas(DataProxy* p);
    VoxelsCanvas(Bucket b);
    VoxelsCanvas(size_t w, size_t h, size_t s, size_t f);
    VoxelsCanvas(const VoxelsCanvas& vc);
    VoxelsCanvas(const VoxelsCanvas& vc, const Bucket& b);

    float at(size_t c, size_t l, size_t s, size_t f) const;
    float at(const Bucket::Iterator& it) const;
    void  set(size_t c, size_t l, size_t s, size_t f, float val);
    void  set(const Bucket::Iterator& it, float val);
};


#endif // VOXELS_CANVAS_HPP_INCLUDED
