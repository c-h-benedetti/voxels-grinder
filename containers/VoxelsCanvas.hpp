#ifndef VOXELS_CANVAS_TEMPLATE_HPP_INCLUDED
#define VOXELS_CANVAS_TEMPLATE_HPP_INCLUDED

#include <vector>
#include <stdexcept>
#include "Data.hpp"
#include "Bucket.hpp"

template <typename T>
class VoxelsCanvas : public Data {

protected:

    /// Actual data looaded from the disk in a 1D array.
    T* data = nullptr;
    /// The global canvas' size.
    Bucket global;
    /// The portion of the canvas currently loaded.
    Bucket loaded;
    /// Order in which areas will be loaded from the disk in case of stream.
    std::vector<Bucket> loading_zones;
    /// If the data is streamed, this variable contains a low-res version that fits in the RAM so we can have an overview.
    T* low_res = nullptr;

public:

    enum class padding_type {
        BLACK, /// Pad with the value 0.
        DRIFT, /// Pad with the last value encountered.
        LOOP   /// Acts like there is a mirror effect.
    };

public:

    inline Bucket get_global_dimensions() const { return this->global; }
    inline Bucket get_loaded_dimensions() const { return this->loaded; }

    inline T& global_at(size_t l, size_t c, size_t s, size_t ch, size_t fr) { return this->data[this->loaded.at(l, c, s, ch, fr, &this->global)]; }
    inline T& local_at(size_t l, size_t c, size_t s, size_t ch, size_t fr) { return this->data[this->loaded.at(l, c, s, ch, fr)]; }
    inline T& at(size_t i) { return this->data[i]; }

protected:

    inline T* get_data_segment() { return this->data; }
    virtual int allocate_memory(Bucket b) = 0;

protected:

    VoxelsCanvas() = delete;
    VoxelsCanvas(DataProxy* p);
    VoxelsCanvas(Bucket b);
    VoxelsCanvas(size_t h, size_t w, size_t s, size_t c, size_t f);
};


// ----------------------------------------------------------------- //

template <typename T>
VoxelsCanvas<T>::VoxelsCanvas(DataProxy* p) {

}


template <typename T>
VoxelsCanvas<T>::VoxelsCanvas(Bucket b) {
    if ((b.height() == 0) || 
        (b.width() == 0) || 
        (b.nSlices() == 0) || 
        (b.nChannels() == 0) || 
        (b.nFrames() == 0)) { throw std::invalid_argument("No dimension should be 0."); }
    
    // 1. Evaluate the total required size and determine if it will require more RAM than available.
    //    This will maybe require the construction to be performed (at least the allocation) in the subclasses
    //    so we can have the size of data types. => allocate_memory();
}


template <typename T>
VoxelsCanvas<T>::VoxelsCanvas(size_t h, size_t w, size_t s, size_t c, size_t f) : 
    VoxelsCanvas(Bucket({0, h}, {0, w}, {0, s}, {0, c}, {0, f})) {}


#endif // VOXELS_CANVAS_TEMPLATE_HPP_INCLUDED
