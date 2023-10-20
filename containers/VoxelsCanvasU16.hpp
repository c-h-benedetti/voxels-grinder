#ifndef VOXELS_CANVAS_UNSIGNED_INT16_HPP_INCLUDED
#define VOXELS_CANVAS_UNSIGNED_INT16_HPP_INCLUDED

#include "Data.hpp"
#include "VoxelsCanvas.hpp"

using u16_vx_t = uint16_t;

class VoxelsCanvasU16 : public VoxelsCanvas<u16_vx_t> {
public:
    inline d_type dtype() const override { return Data::d_type::v_u16; }
};

#endif // VOXELS_CANVAS_UNSIGNED_INT16_HPP_INCLUDED