#ifndef VOXELS_CANVAS_HPP_INCLUDED
#define VOXELS_CANVAS_HPP_INCLUDED

#include <vector>
#include <utility>
#include "glm.hpp"
#include "Data.hpp"
#include "Bucket.hpp"

class VoxelsCanvas : public Data {

    float* data = nullptr;
    float* visualize = nullptr;
    Bucket global;
    Bucket loaded;
    std::vector<Bucket> loading_zones;

public:

    enum class padding_type {
        BLACK,
        DRIFT,
        LOOP
    };

public:

    // Absolute size for padding.
    void require(glm::ivec3 size, padding_type type);
    // Relative size (percentage) for padding.
    void require(glm::vec3 size, padding_type type);

    VoxelsCanvas* alike() const;

    VoxelsCanvas() = delete;
    VoxelsCanvas(size_t height, size_t width, size_t nSlices=1, size_t nChannels=1, size_t nFrames=1);

    ~VoxelsCanvas();
};

std::vector<Bucket> bucketize(const VoxelsCanvas* vc);

#endif //VOXELS_CANVAS_HPP_INCLUDED

/*

data: Valeurs des voxels représentées comme des float.

visualize: Si l'image est trop grande pour la RAM, contient la pyramide Gaussienne qui permet de faire une visu totale en 3D.

global: Taille totale de l'image telle qu'elle est indiquée dans le fichier.

loaded: Zone de l'image actuellement disponible en mémoire.

Dans le cas d'une petite image, on utilise que le champ 'data'. Il est utilisé pour la représentation et le calcul.
Le module d'affichage doit donc vérifier que le champ visualize est vide avant de faire quoi que ce soit.

*/
