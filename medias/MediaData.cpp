#include "MediaData.hpp"

bool MediaDataInstance::resize(size_t c, size_t s, size_t f) {
    Dimensions& d = this->dimensions;
    size_t current = d.nChannels * d.nSlices * d.nFrames;
    size_t target  = c * s * f;

    if (current != target) { return false; }

    d.nChannels = c;
    d.nSlices   = s;
    d.nFrames   = f;

    return true;
}

MediaDataInstance::MediaDataInstance(const Dimensions& dims): dimensions(dims) {}