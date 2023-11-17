#ifndef VOXELS_CANVAS_FLOAT_HPP_INCLUDED
#define VOXELS_CANVAS_FLOAT_HPP_INCLUDED

#include "Data.hpp"
#include "VoxelsCanvas.hpp"

using flt_vx_t = uint64_t;

class VoxelsCanvasFloat : public VoxelsCanvas<flt_vx_t> {
public:
    inline int run(Task* v, Bucket b) override  { return v->run(this, b); }
    inline d_type dtype() const override { return Data::d_type::v_f; }
};

#endif // VOXELS_CANVAS_FLOAT_HPP_INCLUDED