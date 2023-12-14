#ifndef BUCKET_LOCATION_HPP_INCLUDED
#define BUCKET_LOCATION_HPP_INCLUDED

#include <utility>
#include <cstdlib>
#include <stdexcept>
#include "Calibration.hpp"

/**
 * @brief The Bucket object is meant to represent a region within a VoxelsCanvas without storing any of its data.
 *        It is also used as calibrated a bounding-box in the 3D viewer.
 *
 * Since Buckets are also used in the context of multi-threaded processing, they store an index that could also be interpreted as an ID.
 * It allows to reserve a slot in the output buffer for each region without the use of mutex.
 * 
 * A Bucket also stores indices indicating if it contain some overlap, in which case, it should be accessed but not processed.
 *
 * Each axis must be a size of at least 1. The upper bound is never included in the loops.
 * It implies that each `XXX.second` is strictly greater than `XXX.first`.
 *
 * A bucket can contain some padding on one side, but not on the other.
 * Hence, we don't store only padding size on the x, y and z axes but two values for each axis.
 * A function is responsible for generating the list of "processing buckets" for an image.
 *
 * This object can represent several types of areas:
 *  - Global bucket: It represents the actual size of the entire image.
 *  - Loading bucket: The area from the global image that is actually loaded in RAM. It subdivides the global bucket.
 *  - Processing bucket: How the process is fractioned to be launched on several threads. It subdivides a loading bucket.
 *
 * Each subdivision must cover the whole superior level.
 */

class Bucket {

    std::pair<size_t, size_t> rows; // Indices of first and last row stored.
    std::pair<size_t, size_t> columns; // Indices of first and last columns stored.
    std::pair<size_t, size_t> slices; // Indices of first and last slices stored.
    std::pair<size_t, size_t> frames; // Indices of first and last frames stored.

    std::pair<size_t, size_t> overlap_x; // X overlap on either sides.
    std::pair<size_t, size_t> overlap_y; // Y overlap on either sides.
    std::pair<size_t, size_t> overlap_z; // Z overlap on either sides.

    size_t      index; // ID of the bucket, used to prevent access rate in results buffers.
    glm::vec3   origin; // Origin of the bucket if it is used as a bounding-box.
    Calibration calibration; // Calibration used.
    size_t      channel; // Index of the channel that this bucket represents.

private:

    inline void set_dimension(std::pair<size_t, size_t>& target, const std::pair<size_t, size_t>& input) {
        if (input.first >= input.second) { throw std::invalid_argument("Attempt to set a dimension to 0 or a negative value."); }
        target = input;
    }

public:

    // We don't have an empty constructor as we don't want any dimension to be equal to 0.
    //Bucket() = delete;

    Bucket(size_t rows=1, size_t columns=1, size_t slices=1, size_t frames=1):
        Bucket({0, rows}, {0, columns}, {0, slices}, {0, frames}) {

    }

    Bucket(
        std::pair<size_t, size_t> r,
        std::pair<size_t, size_t> c,
        std::pair<size_t, size_t> s  = {0, 1}, 
        std::pair<size_t, size_t> fr = {0, 1}
    ) {
        this->set_rows(r);
        this->set_columns(c);
        this->set_slices(s);
        this->set_frames(fr);
    }

    inline size_t index_of(size_t c, size_t l, size_t s, size_t f) const { 
        size_t W = this->nVoxelsX();
        size_t H = this->nVoxelsY();
        size_t D = this->nVoxelsZ();
        size_t wh = W*H;
        return wh*D*f + wh*s + W*l+c;
    }

    inline void set_calibration(Calibration c) { this->calibration = c; }

    inline void set_columns(const std::pair<size_t, size_t>& c) { this->set_dimension(columns, c); }
    inline void set_rows(const std::pair<size_t, size_t>& r)    { this->set_dimension(rows, r); }
    inline void set_slices(const std::pair<size_t, size_t>& s)  { this->set_dimension(slices, s); }
    inline void set_frames(const std::pair<size_t, size_t>& f)  { this->set_dimension(frames, f); }
    inline void set_index(size_t i)                             { this->index = i; }
    
    inline size_t nVoxelsX() const  { return columns.second - columns.first; }
    inline size_t nVoxelsY() const  { return rows.second - rows.first; }
    inline size_t nVoxelsZ() const  { return slices.second - slices.first; }
    inline size_t nSlices() const   { return slices.second - slices.first; }
    inline size_t nFrames() const   { return frames.second - frames.first; }
    inline size_t length() const    { return nVoxelsX() * nVoxelsY() * nVoxelsZ() * nFrames(); }
    inline size_t get_index() const { return this->index; }

    inline float width() const    { return (float)this->nVoxelsX() * this->calibration.get_size_x(); }
    inline float height() const   { return (float)this->nVoxelsY() * this->calibration.get_size_y(); }
    inline float depth() const    { return (float)this->nVoxelsZ() * this->calibration.get_size_z(); }
    inline float duration() const { return (float)this->nFrames()  * this->calibration.get_time_interval(); }
};

/** TODO:
 * - [ ] Les nombres de voxels sur chaque axe devraient être stockés plutôt que calculés.
 */


#endif //BUCKET_LOCATION_HPP_INCLUDED
