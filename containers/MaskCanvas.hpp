#ifndef MASK_CANVAS_HPP_INCLUDED
#define MASK_CANVAS_HPP_INCLUDED

#include "VoxelsCanvas.hpp"

using mask_vx_t = uint64_t;

class MaskCanvas : VoxelsCanvas<mask_vx_t> {
public:
    inline d_type dtype() const override { return Data::d_type::v_mk; }
};

#endif // MASK_CANVAS_HPP_INCLUDED