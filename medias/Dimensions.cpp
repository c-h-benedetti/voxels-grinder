#include "Dimensions.hpp"

Dimensions::Dimensions(size_t w, size_t h, bit_depth b, size_t c, size_t s, size_t f):
    width(w),
    height(h),
    depth(b),
    nChannels(c),
    nSlices(s),
    nFrames(f) {}

bool operator==(const Dimensions& d1, const Dimensions& d2) {
    return (d1.width == d2.width) && (d1.height == d2.height) && (d1.nChannels == d2.nChannels) && (d1.nSlices == d2.nSlices) && (d1.nFrames == d2.nFrames);
}

bool operator!=(const Dimensions& d1, const Dimensions& d2) {
    return !(d1 == d2);
}