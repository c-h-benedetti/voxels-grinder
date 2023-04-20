#include "Dimensions.hpp"

Dimensions::Dimensions(size_t w, size_t h, bit_depth b, size_t c, size_t s, size_t f):
    width(w),
    height(h),
    depth(b),
    nChannels(c),
    nSlices(s),
    nFrames(f) {}
