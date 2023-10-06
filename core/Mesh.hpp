#ifndef MESH_HPP_INCLUDED
#define MESH_HPP_INCLUDED

#include "Data.hpp"

class Mesh : public Vertices {
    std::vector<glm::ivec3> triangles;
};

#endif // MESH_HPP_INCLUDED