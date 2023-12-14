#ifndef VOXELS_CANVAS_HPP_INCLUDED
#define VOXELS_CANVAS_HPP_INCLUDED

#include <vector>
#include <filesystem>
#include <stdexcept>
#include "core/Data.hpp"
#include "core/Bucket.hpp"

class VoxelsCanvas : public Data {

protected:

    /// Actual data looaded from the disk in a 1D array.
    float* data = nullptr;
    /// The global canvas' size.
    Bucket global;
    /// The portion of the canvas currently loaded.
    Bucket loaded;
    /// If the data is streamed, this variable contains a low-res version that fits in the RAM so we can have an overview.
    float* low_res = nullptr;

protected:

    inline float* get_data_segment() { return this->data; }
    bool allocate_memory(Bucket b);

public:

    enum class DisplayFlags : uint64_t {
        image  = 0,
        mask   = 1 << 0,
        labels = 1 << 1
    };

    enum class padding_type {
        BLACK, /// Pad with the value 0.
        DRIFT, /// Pad with the last value encountered.
        LOOP   /// Acts like there is a mirror effect.
    };

    inline Bucket get_global_dimensions() const { return this->global; }
    inline Bucket get_loaded_dimensions() const { return this->loaded; }
    int run(Task* v, Bucket b) override;

    VoxelsCanvas() = delete;
    VoxelsCanvas(DataProxy* p);
    VoxelsCanvas(Bucket b);
    VoxelsCanvas(size_t h, size_t w, size_t s, size_t f);
    VoxelsCanvas(const VoxelsCanvas& vc);
    VoxelsCanvas(const VoxelsCanvas& vc, const Bucket& b);

    float& at(size_t c, size_t l, size_t s, size_t f);
    void  set(size_t c, size_t l, size_t s, size_t f, float val);

    void to_dump(const std::filesystem::path& p);
    void from_dump(const std::filesystem::path& p);

};


#endif // VOXELS_CANVAS_HPP_INCLUDED
