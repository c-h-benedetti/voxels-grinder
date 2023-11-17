#ifndef BUCKET_LOCATION_HPP_INCLUDED
#define BUCKET_LOCATION_HPP_INCLUDED

#include <utility>
#include <cstdlib>

/**
 * @brief The Bucket object is meant to represent a region within a VoxelsCanvas without storing any of its data.
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

    std::pair<size_t, size_t> rows;
    std::pair<size_t, size_t> columns;
    std::pair<size_t, size_t> slices;
    std::pair<size_t, size_t> channels;
    std::pair<size_t, size_t> frames;

    std::pair<size_t, size_t> overlap_x;
    std::pair<size_t, size_t> overlap_y;
    std::pair<size_t, size_t> overlap_z;

    size_t index;

public:

    Bucket() = default;

    Bucket(std::pair<size_t, size_t> c, std::pair<size_t, size_t> r, std::pair<size_t, size_t> s={0, 1}, std::pair<size_t, size_t> ch={0, 1}, std::pair<size_t, size_t> fr={0, 1}) {
        this->set_rows(r);
        this->set_columns(c);
        this->set_slices(s);
        this->set_channels(ch);
        this->set_frames(fr);
    }
    
    inline void set_rows(const std::pair<size_t, size_t>& r)     { rows.first     = r.first; rows.second     = (r.first < r.second) ? (r.second) : (r.first+1); }
    inline void set_columns(const std::pair<size_t, size_t>& c)  { columns.first  = c.first; columns.second  = (c.first < c.second) ? (c.second) : (c.first+1); }
    inline void set_slices(const std::pair<size_t, size_t>& s)   { slices.first   = s.first; slices.second   = (s.first < s.second) ? (s.second) : (s.first+1); }
    inline void set_channels(const std::pair<size_t, size_t>& c) { channels.first = c.first; channels.second = (c.first < c.second) ? (c.second) : (c.first+1); }
    inline void set_frames(const std::pair<size_t, size_t>& f)   { frames.first   = f.first; frames.second   = (f.first < f.second) ? (f.second) : (f.first+1); }
    
    inline size_t height() const    { return rows.second - rows.first; }
    inline size_t width() const     { return columns.second - columns.first; }
    inline size_t nSlices() const   { return slices.second - slices.first; }
    inline size_t nChannels() const { return channels.second - channels.first; }
    inline size_t nFrames() const   { return frames.second - frames.first; }
};

#endif //BUCKET_LOCATION_HPP_INCLUDED
