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
};

#endif // MEDIA_DIMENSIONS_HPP_INCLUDED