#ifndef POLYLINE_HPP_INCLUDED
#define POLYLINE_HPP_INCLUDED

#include <utility>
#include "Vertices.hpp"

class PolyLine : public Vertices {
    bool closed;
    std::vector<std::pair<size_t, size_t>> segments;
};

#endif // POLYLINE_HPP_INCLUDED