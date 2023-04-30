#ifndef DATA_UTILS_HPP_INCLUDED
#define DATA_UTILS_HPP_INCLUDED

#include "MediaData.hpp"

struct DataUtils {

    // -------- Instanciators ------------------------------------------------------------

    static ByteData createByteData(const Dimensions& d);
    static ShortData createShortData(const Dimensions& d);
    static FloatData createFloatData(const Dimensions& d);
    static RGBData createRGBData(const Dimensions& d);
    static MediaData duplicate(const MediaData ori);

    template <typename T>
    static T duplicateAs(const T ori) {
        return nullptr;
    }

    // -------- From byte ----------------------------------------------------------------

    static ByteData convertToByte(ByteData p);
    static ShortData convertToShort(ByteData p);
    static FloatData convertToFloat(ByteData p);
    static RGBData convertToRGB(ByteData p);

    // -------- From short ---------------------------------------------------------------

    static ByteData convertToByte(ShortData p);
    static ShortData convertToShort(ShortData p);
    static FloatData convertToFloat(ShortData p);
    static RGBData convertToRGB(ShortData p);

    // -------- From float ---------------------------------------------------------------

    static ByteData convertToByte(FloatData p);
    static ShortData convertToShort(FloatData p);
    static FloatData convertToFloat(FloatData p);
    static RGBData convertToRGB(FloatData p);

    // -------- From RGB -----------------------------------------------------------------

    static ByteData convertToByte(RGBData p);
    static ShortData convertToShort(RGBData p);
    static FloatData convertToFloat(RGBData p);
    static RGBData convertToRGB(RGBData p);
};

#endif // DATA_UTILS_HPP_INCLUDED