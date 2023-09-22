#ifndef ALL_DATA_TYPES_HPP_INCLUDED
#define ALL_DATA_TYPES_HPP_INCLUDED

#include <glm.hpp>

/**
 * This header forward-declares every data types available in VoxelsGrinder.
*/

class Data;

template <typename T>
class Image;

class ImageMask;

class Image8;

class Image16;

class ImageFloat;

class ImageLabels;

class ImageRGB;

using i_type_8      = uint8_t;
using i_type_mask   = bool;
using i_type_16     = uint16_t;
using i_type_float  = float;
using i_type_labels = uint32_t;
using i_type_rgb    = glm::vec3;

#endif // ALL_DATA_TYPES_HPP_INCLUDED