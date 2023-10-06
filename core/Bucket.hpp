#ifndef BUCKET_LOCATION_HPP_INCLUDED
#define BUCKET_LOCATION_HPP_INCLUDED

#include <utility>
#include <cstdlib>

class Bucket {

    std::pair<size_t, size_t> rows;
    std::pair<size_t, size_t> columns;
    std::pair<size_t, size_t> slices;
    std::pair<size_t, size_t> channels;
    std::pair<size_t, size_t> frames;

    std::pair<size_t, size_t> overlap_x;
    std::pair<size_t, size_t> overlap_y;
    std::pair<size_t, size_t> overlap_z;

public:

    Bucket() = default;

    Bucket(
        std::pair<size_t, size_t> r, 
        std::pair<size_t, size_t> c, 
        std::pair<size_t, size_t> s={0, 1},
        std::pair<size_t, size_t> ch={0, 1},
        std::pair<size_t, size_t> fr={0, 1}) : rows(r), columns(c), slices(s), channels(ch), frames(fr) {}
    
    inline size_t height() const    { return rows.second - rows.first; }
    inline size_t width() const     { return columns.second - columns.first; }
    inline size_t nSlices() const   { return slices.second - slices.first; }
    inline size_t nChannels() const { return channels.second - channels.first; }
    inline size_t nFrames() const   { return frames.second - frames.first; }
};

#endif //BUCKET_LOCATION_HPP_INCLUDED