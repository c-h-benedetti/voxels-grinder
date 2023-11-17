#ifndef VERTICES_HPP_INCLUDED
#define VERTICES_HPP_INCLUDED

#include <vector>
#include "Data.hpp"
#include "glm.hpp"

class Vertices : public Data {

protected:
    std::vector<glm::vec3> vertices;

public:

    inline int run(Task* v, Bucket b) override  { return v->run(this, b); }
    inline d_type dtype() const override { return Data::d_type::vertices; }
};

#endif // VERTICES_HPP_INCLUDED