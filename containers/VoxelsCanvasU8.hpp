#ifndef VOXELS_CANVAS_UNSIGNED_INT8_HPP_INCLUDED
#define VOXELS_CANVAS_UNSIGNED_INT8_HPP_INCLUDED

#include "Data.hpp"
#include "VoxelsCanvas.hpp"

using u8_vx_t = uint8_t;

class VoxelsCanvasU8 : public VoxelsCanvas<u8_vx_t> {
public:
    inline d_type dtype() const override { return Data::d_type::v_u8; }
};

#endif // VOXELS_CANVAS_UNSIGNED_INT8_HPP_INCLUDED