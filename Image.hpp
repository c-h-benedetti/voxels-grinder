#ifndef IMAGE_CONTAINER_IO_HPP_INCLUDED
#define IMAGE_CONTAINER_IO_HPP_INCLUDED

#include <filesystem>
#include "ImageUtils.hpp"

#define IMAGE_DEFAULT_NAME "Anonymous"

template <typename T>
class Image {

    /// @brief Unique name associated to this instance of Image.
    std::filesystem::path name = IMAGE_DEFAULT_NAME;
    size_t height;
    size_t width;
    size_t slices;
    uint8_t channels;
    uint8_t frames;
    std::vector<T*> voxelsGrids;

public:

    /// @return The unique name associated to this Image.
    inline const std::filesystem::path& getName() { return this->name; }

    /// @brief Changes the name of this instance of image if \c p is not already given. The old name is conserved if the operation is not possible.
    /// @param p The name to be attributed to this image.
    /// @param acceptIndex Can the provided name be concatenated with an index if it is not available (ex: \c "MyName" into \c "MyName-0001").
    /// @return \c 0 if the operation is a success
    /// @return \c -1 if failed to change the name.
    int setName(const std::filesystem::path& p, bool acceptIndex=false);

    /// @return The height (y-axis) of the image.
    size_t getHeight() const;

    /// @return The width (x-axis) of the image.
    size_t getWidth() const;

    /// @return The depth (z-axis) of the image, that corresponds to the number of slices.
    size_t getDepth() const;

    /// @return Same thing as Image::getDepth().
    size_t getSlices() const;

    /// @return The number of frames contained in the image.
    size_t getFrames() const;

    /// @return The number of channels of this image.
    uint8_t getChannels() const;

    /// @return The number of bits on which each voxel is encoded.
    inline uint8_t getBitDepth() const { return sizeof(T); }

    /// @brief Writes the image to the disk at the specified path.
    /// @param p A complete path including a name and a extension for the image to be exported (ex: "my/path/for/image.png").
    /// @param safeExport Cancels the export if the file already exists on the disk.
    int write(const std::filesystem::path& p, bool safeExport=false);

    /// @brief Writes the image to the disk at the specified path. The complete path is built from the provided path, the name of the image and the provided extension.
    /// @param p The path of an existing folder.
    /// @param format The format to which this image must be exported.
    /// @param safeExport Cancels the export if the file already exists on the disk.
    int write(const std::filesystem::path& p, ImageUtils::ImageFormat format, bool safeExport=false);

public:

    Image() = default;
    Image(const std::filesystem::path& p);
    Image(const size_t& height, const size_t& width, const size_t& slices=1, const size_t& channels=1, const size_t& frames=1, const T& defaultVal=0);

    Image(const Image& img);

    template <typename U>
    Image(const Image<U>& img);

    ~Image();
};



// # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
// # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
// # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

#include <algorithm>
#include <cstring>
#include "utils.hpp"

//> [ImageUtils::NOTE] Concurrent access requires securing the vector containing all images' names.
template <typename T>
int Image<T>::setName(const std::filesystem::path& p, bool acceptIndex) {
    auto it = std::lower_bound(
        ImageUtils::attributedNames.begin(),
        ImageUtils::attributedNames.end(),
        p
    );

    // Name already given.
    if (*it == p) { return -1; }

    auto it2 = std::lower_bound(
        ImageUtils::attributedNames.begin(),
        ImageUtils::attributedNames.end(),
        this->name
    );

    ImageUtils::attributedNames.erase(it2);
    this->name = p;
    insert_sorted(ImageUtils::attributedNames, p);

    return 0;
}

template <typename T>
Image<T>::Image(const size_t& height, const size_t& width, const size_t& slices, const size_t& channels, const size_t& frames, const T& defaultVal): height(height), width(width), slices(slices), channels(channels), frames(frames) {
    // Les slices doivent être stockées entrelacées dans la mesure où elles doivent intéragir.
    // Ne pas oublier d'utiliser la valeur par default fournie dans les paramètres du constructeur.
    // Est-ce qu'on veut pouvoir ajouter des channels, des slices, des frames, ...?
    
    // La hauteur, la largeur et la hauteur (slices) sont entrelacées (dans le même segment).
    // Les channels sont stockés côté-à-côte dans le vector. S'il y a N channels v[K*N+I] sont les channels d'une même frame
    //  K is a natural number.
    //  N is the number of channels (natural without 0).
    //  I is in [0, N-1].
    // Les frames sont stockées côte-à-côte, mais tous les channels d'une frame doivent être écoulés avant de passer à l'autre.
    // Les slices n'ont pas vraiment à être entrelacées pour l'instant, on peut simplement les mettre l'une à la suite de l'autre.
    // Il faudra tester quelle méthode d'entrelacement est plus rapide pour le calcul.
    // Écrire du code de sécurité pour ne pas pouvoir donner des 0.

    const size_t sizeOfGrid = this->width * this->height * this->slices;

    for (size_t f = 0 ; f < this->frames ; f++) {
        for (uint8_t c = 0 ; c < this->channels ; c++) {
            T* data = new T[sizeOfGrid];
            memset(data, defaultVal, sizeOfGrid);
            this->voxelsGrids.push_back(data);
        }
    }
}

template <typename T>
Image<T>::~Image() {
    for (T*& img : this->voxelsGrids) {
        delete[] img;
        img = nullptr;
    }
    this->voxelsGrids.clear();
}

#endif // IMAGE_IO_HPP_INCLUDED