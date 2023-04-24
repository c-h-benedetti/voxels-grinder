#ifndef MEDIA_DATA_HPP_INCLUDED
#define MEDIA_DATA_HPP_INCLUDED

#include <vector>
#include <memory>
#include <utility>
#include "Dimensions.hpp"


class MediaDataInstance {

protected:

    Dimensions dimensions;

public:

    virtual bool resize(size_t c, size_t s, size_t f);

    inline Dimensions getDimensions() const { return this->dimensions; }

    MediaDataInstance() = delete;
    MediaDataInstance(const Dimensions& dims);

    virtual ~MediaDataInstance() = default;
};

using MediaData = std::shared_ptr<MediaDataInstance>;

template <typename T>
class MediaDataArena : public MediaDataInstance {

protected:

    std::vector<T> data;

public:

    T& at(size_t r, size_t c, size_t ch, size_t sl, size_t fr) {
        return this->data[this->dimensions.indexOf(r, c, ch, sl, fr)];
    }

    T& at(size_t i) {
        return this->data[i];
    }

    T get(size_t r, size_t c, size_t ch, size_t sl, size_t fr) const {
        return this->data[this->dimensions.indexOf(r, c, ch, sl, fr)];
    }

    T get(size_t i) const {
        return this->data[i];
    }

    std::pair<size_t, size_t> imageRange(size_t ch, size_t sl, size_t fr) const {
        return {
            this->dimensions.indexOf(0, 0, ch, sl, fr),
            this->dimensions.indexOf(this->dimensions.height-1, this->dimensions.width-1, ch, sl, fr)
        };
    }

    inline std::vector<T>& getRawData() { return this->data; }
    inline const std::vector<T>& getRawData() const { return this->data; }
    
    MediaDataArena() = delete;
    MediaDataArena(const Dimensions& dims): MediaDataInstance(dims) {
        const size_t nItems = dims.width * dims.height * dims.nChannels * dims.nSlices * dims.nFrames;
        this->data.clear();
        this->data.resize(nItems, 0);
    }

    virtual ~MediaDataArena() = default;
};


class ByteDataArena : public MediaDataArena<uint8_t> {
public:
    ByteDataArena() = delete;
    ByteDataArena(const Dimensions& dims): MediaDataArena<uint8_t>(dims) {}
};

class ShortDataArena : public MediaDataArena<uint16_t> {
public:
    ShortDataArena() = delete;
    ShortDataArena(const Dimensions& dims): MediaDataArena<uint16_t>(dims) {}
};

class FloatDataArena : public MediaDataArena<float> {
public:
    FloatDataArena() = delete;
    FloatDataArena(const Dimensions& dims): MediaDataArena<float>(dims) {}
};

class RGBDataArena : public MediaDataArena<uint8_t> {
public:
    RGBDataArena() = delete;
    RGBDataArena(const Dimensions& dims): MediaDataArena<uint8_t>(dims) {}
};

using ByteData  = std::shared_ptr<ByteDataArena>;
using ShortData = std::shared_ptr<ShortDataArena>;
using FloatData = std::shared_ptr<FloatDataArena>;
using RGBData   = std::shared_ptr<RGBDataArena>;

#endif // MEDIA_DATA_HPP_INCLUDED