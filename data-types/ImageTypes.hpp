#ifndef VOXEL_IMAGE_TYPES_HPP_INCLUDED
#define VOXEL_IMAGE_TYPES_HPP_INCLUDED

#include "data-types.hpp"
#include "Image.hpp"

class ImageMask : public Image<i_type_mask> {};

class Image8 : public Image<i_type_8> {};

class Image16 : public Image<i_type_16> {};

class ImageFloat : public Image<i_type_float> {};

class ImageLabels : public Image<i_type_labels> {};

class ImageRGB : public Image<i_type_rgb> {};


#endif // VOXEL_IMAGE_TYPES_HPP_INCLUDED