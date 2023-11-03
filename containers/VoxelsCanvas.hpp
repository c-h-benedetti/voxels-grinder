#ifndef VOXELS_CANVAS_TEMPLATE_HPP_INCLUDED
#define VOXELS_CANVAS_TEMPLATE_HPP_INCLUDED

#include <vector>
#include "Data.hpp"
#include "Bucket.hpp"

template <typename T>
class VoxelsCanvas : public Data {

protected:

    T* data = nullptr;
    Bucket global;
    Bucket loaded;
    std::vector<Bucket> loading_zones;

public:

    enum class padding_type {
        BLACK,
        DRIFT,
        LOOP
    };

public:

    inline Bucket get_global_dimensions() const { return this->global; }

protected:

    inline T* get_data_segment() { return this->data; }

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

}


template <typename T>
VoxelsCanvas<T>::VoxelsCanvas(size_t h, size_t w, size_t s, size_t c, size_t f) {

}


#endif // VOXELS_CANVAS_TEMPLATE_HPP_INCLUDED
