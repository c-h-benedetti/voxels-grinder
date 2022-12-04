#include "ImageUtils.hpp"

std::vector<std::filesystem::path> ImageUtils::attributedNames = {};

const std::map<const char*, ImageUtils::ImageFormat> ImageUtils::handledFormats = {
    {"png", ImageUtils::ImageFormat::PNG},
    {"tif", ImageUtils::ImageFormat::TIFF},
    {"tiff", ImageUtils::ImageFormat::TIFF},
    {"pgm", ImageUtils::ImageFormat::PGM},
    {"ppm", ImageUtils::ImageFormat::PPM},
    {"jpg", ImageUtils::ImageFormat::JPG},
    {"jpeg", ImageUtils::ImageFormat::JPG},
};
