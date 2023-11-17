#ifndef LABELED_CANVAS_HPP_INCLUDED
#define LABELED_CANVAS_HPP_INCLUDED

#include "VoxelsCanvas.hpp"

using lbld_vx_t = uint64_t;

class LabeledCanvas : VoxelsCanvas<lbld_vx_t> {
public:
    inline int run(Task* v, Bucket b) override  { return v->run(this, b); }
    inline d_type dtype() const override { return Data::d_type::v_lbld; }
};

#endif // LABELED_CANVAS_HPP_INCLUDED