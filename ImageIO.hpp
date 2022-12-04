#ifndef IMAGE_IO_HPP_INCLUDED
#define IMAGE_IO_HPP_INCLUDED

#include <filesystem>
#include "Image.hpp"

template <typename T>
class ImageIO {

public:

    virtual void exportImage(const Image<T>& i) = 0;
    virtual std::filesystem::path getExtension() = 0;
};

#endif // IMAGE_IO_HPP_INCLUDED
