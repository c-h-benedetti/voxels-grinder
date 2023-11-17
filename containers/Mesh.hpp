#ifndef MESH_HPP_INCLUDED
#define MESH_HPP_INCLUDED

#include "Vertices.hpp"

struct Triangle {
    uint32_t indices[3] = {};
};

class Mesh : public Vertices {
    std::vector<Triangle> triangles;
public:
    inline int run(Task* v, Bucket b) override  { return v->run(this, b); }
    inline d_type dtype() const override { return Data::d_type::mesh; }
};

#endif // MESH_HPP_INCLUDED