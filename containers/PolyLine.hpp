#ifndef POLYLINE_HPP_INCLUDED
#define POLYLINE_HPP_INCLUDED

#include "Vertices.hpp"

class PolyLine : public Vertices {

protected:

    bool closed = false;

public:

    inline d_type dtype() const override { return Data::d_type::polygon; }
};

#endif // POLYLINE_HPP_INCLUDED