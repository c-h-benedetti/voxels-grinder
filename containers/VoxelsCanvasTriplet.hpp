#ifndef VOXELS_CANVAS_TRIPLET_HPP_INCLUDED
#define VOXELS_CANVAS_TRIPLET_HPP_INCLUDED

#include "Data.hpp"
#include "VoxelsCanvas.hpp"

using trp_vx_t = uint8_t;

struct Triplet {
    trp_vx_t values[3] = {};

    inline trp_vx_t& x() { return values[0]; }
    inline trp_vx_t& y() { return values[1]; }
    inline trp_vx_t& z() { return values[2]; }
};

class VoxelsCanvasTriplet : public VoxelsCanvas<Triplet> {
public:
    inline d_type dtype() const override { return Data::d_type::v_tpl; }
};

#endif // VOXELS_CANVAS_TRIPLET_HPP_INCLUDED