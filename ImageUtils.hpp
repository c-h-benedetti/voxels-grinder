#ifndef IMAGE_UTILS_HPP_INCLUDED
#define IMAGE_UTILS_HPP_INCLUDED

#include <vector>
#include <map>
#include <filesystem>

class ImageUtils {

    template <typename U>
    friend class Image;

    enum class ImageFormat {
        UNSET,
        PNG,
        TIFF,
        PGM,
        PPM,
        JPG
    };

    /// @brief Map associating extensions under the form of a chain of chars to an identifier.
    static const std::map<const char*, ImageFormat> handledFormats;

    /// @brief List of names of all currently opened images. Two images cannot have the same name; An instance's destruction releases the availability of its name.
    static std::vector<std::filesystem::path> attributedNames;
};

#endif // IMAGE_UTILS_HPP_INCLUDED