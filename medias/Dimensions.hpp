#ifndef MEDIA_DIMENSIONS_HPP_INCLUDED
#define MEDIA_DIMENSIONS_HPP_INCLUDED

#include "bit_depth.hpp"
#include <cstddef>

class Dimensions {
public:

    size_t width;
    size_t height;
    bit_depth depth;
    size_t nChannels;
    size_t nSlices;
    size_t nFrames;

    Dimensions(size_t w=500, size_t h=500, bit_depth b=bit_depth::BYTE, size_t c=1, size_t s=1, size_t f=1);

    inline size_t indexOf(size_t r, size_t c, size_t ch, size_t sl, size_t fr) const {
        size_t K = width * height;
        return (fr * nSlices * nChannels * K) + (ch * nSlices * K) + (sl * K) + (r * width) + c;
    }

    inline size_t nItems() const { return width * height * nChannels * nSlices * nFrames; }
};

bool operator==(const Dimensions& d1, const Dimensions& d2);
bool operator!=(const Dimensions& d1, const Dimensions& d2);

#endif // MEDIA_DIMENSIONS_HPP_INCLUDED