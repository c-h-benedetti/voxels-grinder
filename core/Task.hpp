#ifndef BASE_TASK_HPP_INCLUDED
#define BASE_TASK_HPP_INCLUDED

#include <string>
#include <stdexcept>
#include "Data.hpp"
#include "Bucket.hpp"

class unsupported_operation : public std::logic_error {
public:
    unsupported_operation(const std::string& message)
        : std::logic_error(message) {}
};

struct Task {
    
    Task() = default;
    virtual ~Task() = default;

    virtual int run(const Data*, Bucket b);

    virtual int run(const MaskCanvas*, Bucket b);
    virtual int run(const LabeledCanvas*, Bucket b);

    virtual int run(const VoxelsCanvasU8*, Bucket b);
    virtual int run(const VoxelsCanvasU16*, Bucket b);
    virtual int run(const VoxelsCanvasFloat*, Bucket b);
    virtual int run(const VoxelsCanvasTriplet*, Bucket b);

    virtual int run(const Vertices*, Bucket b);
    virtual int run(const PolyLine*, Bucket b);
    virtual int run(const Mesh*, Bucket b);

    virtual int execute(Bucket b) = 0;
};

#endif // BASE_TASK_HPP_INCLUDED
