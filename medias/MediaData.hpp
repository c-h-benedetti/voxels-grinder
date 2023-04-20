#ifndef MEDIA_DATA_HPP_INCLUDED
#define MEDIA_DATA_HPP_INCLUDED

#include <vector>
#include <memory>
#include "Dimensions.hpp"

/**
 * TODO: Expliquer que dans ce soft, les images sont groupées par channel (pour avoir tous les niveaux d'un même channel) plutôt que par slice (qui stocke tous les channels d'un niveau précis).
 */
class MediaDataInstance {
    
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

    std::vector<T> data;

public:

    inline std::vector<T>& getRawData() { return this->data; }
    inline const std::vector<T>& getRawData() const { return this->data; }
    
    MediaDataArena() = delete;
    MediaDataArena(const Dimensions& dims): MediaDataInstance(dims) {}

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

using ByteData = std::shared_ptr<ByteDataArena>;
using ShortData = std::shared_ptr<ShortDataArena>;
using FloatData = std::shared_ptr<FloatDataArena>;
using RGBData = std::shared_ptr<RGBDataArena>;

#endif // MEDIA_DATA_HPP_INCLUDED