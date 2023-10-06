#ifndef VOXELS_CANVAS_HPP_INCLUDED
#define VOXELS_CANVAS_HPP_INCLUDED

#include <utility>
#include "glm.hpp"
#include "Data.hpp"
#include "Bucket.hpp"

#define voxel_false 0.0f
#define voxel_true  1.0f

class VoxelsCanvas : public Data {

    float* data = nullptr;
    Bucket global;
    Bucket loaded;

public:

    enum class padding_type {
        BLACK,
        MIRROR,
        DRIFT
    };

public:

    // Absolute size for padding.
    void require(glm::ivec3 size, padding_type type);
    // Relative size (percentage) for padding.
    void require(glm::vec3 size, padding_type type);

    VoxelsCanvas() = delete;
    VoxelsCanvas(size_t height, size_t width, size_t nSlices=1, size_t nChannels=1, size_t nFrames=1);

    ~VoxelsCanvas();
};

#endif //VOXELS_CANVAS_HPP_INCLUDED

/*

data: Valeurs des voxels représentées comme des float.

global: Taille totale de l'image telle qu'elle est indiquée dans le fichier.

loaded: Zone de l'image actuellement disponible en mémoire.

*/
