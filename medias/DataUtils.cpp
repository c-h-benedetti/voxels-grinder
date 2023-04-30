#include "DataUtils.hpp"

// -------- Instanciators ------------------------------------------------------------

ByteData DataUtils::createByteData(const Dimensions& d) {
    return ByteData(new ByteDataArena(d));
}

ShortData DataUtils::createShortData(const Dimensions& d) {
    return ShortData(new ShortDataArena(d));
}

FloatData DataUtils::createFloatData(const Dimensions& d) {
    return FloatData(new FloatDataArena(d));
}

RGBData DataUtils::createRGBData(const Dimensions& d) {
    Dimensions d2 = d;
    d2.nChannels = 3;
    return RGBData(new RGBDataArena(d2));
}

MediaData DataUtils::duplicate(const MediaData ori) {
    return nullptr;
}

// -------- From byte ----------------------------------------------------------------

/**
    * @brief Duplicates a ByteData.
    * @param p An instance of ByteData. It remains untouched along the process.
    * @return A new instance of ByteData.
    */
ByteData DataUtils::convertToByte(ByteData p) {
    return DataUtils::duplicateAs<ByteData>(p);
}

/**
    * @brief Creates a new ShortData from an instance of ByteData.
    * @param p An instance of ByteData. It remains untouched along the process.
    * @return A new instance of ShortData.
    */
ShortData DataUtils::convertToShort(ByteData p) {
    Dimensions d = p->getDimensions();
    ShortData s = DataUtils::createShortData(p->getDimensions());
    const std::vector<uint8_t>&  p_data = p->getRawData();
    std::vector<uint16_t>& s_data = s->getRawData();
    size_t K = d.width * d.height;

    for (size_t f = 0 ; f < d.nFrames ; f++) {
        size_t fShift = f * d.nSlices * d.nChannels * K;
        for (size_t ch = 0 ; ch < d.nChannels ; ch++) {
            size_t chShift = ch * d.nChannels * K;
            for (size_t s = 0 ; s < d.nSlices ; s++) {
                size_t sShift = s * K;
                for (size_t r = 0 ; r < d.height ; r++) {
                    size_t rShift = r * d.width;
                    for (size_t c = 0 ; c < d.width ; c++) {
                        size_t idx = fShift + chShift + sShift + rShift + c;
                        s_data[idx] = static_cast<uint16_t>(p_data[idx]);
                    }
                }
            }
        }
    }

    return s;
}

/**
    * @brief Creates a new FloatData from an instance of ByteData.
    * @param p An instance of ByteData. It remains untouched along the process.
    * @return A new instance of FloatData.
    */
FloatData DataUtils::convertToFloat(ByteData p) {
    Dimensions d         = p->getDimensions();
    FloatData s = DataUtils::createFloatData(p->getDimensions());
    const std::vector<uint8_t>&  p_data     = p->getRawData();
    std::vector<float>& s_data     = s->getRawData();
    size_t K             = d.width * d.height;

    for (size_t f = 0 ; f < d.nFrames ; f++) {
        size_t fShift = f * d.nSlices * d.nChannels * K;
        for (size_t ch = 0 ; ch < d.nChannels ; ch++) {
            size_t chShift = ch * d.nChannels * K;
            for (size_t s = 0 ; s < d.nSlices ; s++) {
                size_t sShift = s * K;
                for (size_t r = 0 ; r < d.height ; r++) {
                    size_t rShift = r * d.width;
                    for (size_t c = 0 ; c < d.width ; c++) {
                        size_t idx = fShift + chShift + sShift + rShift + c;
                        s_data[idx] = static_cast<float>(p_data[idx]);
                    }
                }
            }
        }
    }

    return s;
}

/**
    * @brief Creates a new RGBData from an instance of ByteData.
    * An RGBData has a constant number of channels, which is 3.
    * Hence, only images with 1 to 3 channels can be casted to this type.
    * Also the bit depth per channel is set to 8 and cannot be changed.
    * Contrary to other types of images, RGB has their 3 channels interlaced.
    * - Your image can have up to 3 channels, associated channel in the RGB will be filled.
    * - Empty channels are left black.
    * - If the input Data has more than 3 channels, an error will occur.
    * @param p An instance of ByteData. It remains untouched along the process.
    * @return A new instance of FloatData.
    */
RGBData DataUtils::convertToRGB(ByteData p) {
    Dimensions d = p->getDimensions();

    if (d.nChannels > 3) {
        throw "Too many channels in input Media";
    }

    RGBData s = DataUtils::createRGBData(p->getDimensions());
    const std::vector<uint8_t>&  p_data = p->getRawData();
    std::vector<uint8_t>& s_data = s->getRawData();
    size_t K = 3 * d.width * d.height;
    size_t Q = d.width * d.height;

    for (size_t f = 0 ; f < d.nFrames ; f++) {
        size_t fShift = f * d.nSlices * K;
        for (size_t s = 0 ; s < d.nSlices ; s++) {
            size_t sShift = s * K;
            for (size_t r = 0 ; r < d.height ; r++) {
                size_t rShift = r * d.width * 3;
                for (size_t c = 0 ; c < d.width ; c++) {
                    size_t cShift = 3 * c;
                    for (size_t ch = 0 ; ch < d.nChannels ; ch++) {
                        size_t dstIdx = fShift + sShift + rShift + cShift + ch;
                        size_t srcIdx = (f * d.nSlices * d.nChannels * Q) + (ch * d.nChannels * Q) + (s * Q) + (r * d.width) + c;
                        s_data[dstIdx] = p_data[srcIdx];
                    }
                }
            }
        }
    }
    
    return s;
}

// -------- From short ---------------------------------------------------------------

ByteData DataUtils::convertToByte(ShortData p) {
    ByteData s = DataUtils::createByteData(p->getDimensions());
    return s;
}

ShortData DataUtils::convertToShort(ShortData p) {
    return DataUtils::duplicateAs<ShortData>(p);
}

FloatData DataUtils::convertToFloat(ShortData p) {
    FloatData s = DataUtils::createFloatData(p->getDimensions());
    return s;
}

RGBData DataUtils::convertToRGB(ShortData p) {
    RGBData s = DataUtils::createRGBData(p->getDimensions());
    return s;
}

// -------- From float ---------------------------------------------------------------

ByteData DataUtils::convertToByte(FloatData p) {
    ByteData s = DataUtils::createByteData(p->getDimensions());
    return s;
}

ShortData DataUtils::convertToShort(FloatData p) {
    ShortData s = DataUtils::createShortData(p->getDimensions());
    return s;
}

FloatData DataUtils::convertToFloat(FloatData p) {
    return DataUtils::duplicateAs<FloatData>(p);
}

RGBData DataUtils::convertToRGB(FloatData p) {
    RGBData s = DataUtils::createRGBData(p->getDimensions());
    return s;
}

// -------- From RGB -----------------------------------------------------------------

ByteData DataUtils::convertToByte(RGBData p) {
    ByteData s = DataUtils::createByteData(p->getDimensions());
    return s;
}

ShortData DataUtils::convertToShort(RGBData p) {
    ShortData s = DataUtils::createShortData(p->getDimensions());
    return s;
}

FloatData DataUtils::convertToFloat(RGBData p) {
    FloatData s = DataUtils::createFloatData(p->getDimensions());
    return s;
}

RGBData DataUtils::convertToRGB(RGBData p) {
    return DataUtils::duplicateAs<RGBData>(p);
}
