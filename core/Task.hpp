#ifndef BASE_TASK_HPP_INCLUDED
#define BASE_TASK_HPP_INCLUDED

#include <stdexcept>
#include <vector>
#include "Data.hpp"
#include "Bucket.hpp"

class unsupported_operation : public std::logic_error {
public:
    unsupported_operation(const char* message)
        : std::logic_error(message) {}
};

struct Task {

    std::vector<Bucket> buckets;

    Task() = default;
    virtual ~Task() = default;

    virtual int run(Bucket b);
    virtual int run(const Data*, Bucket b);
    virtual int run(const VoxelsCanvas*, Bucket b);
    virtual int run(const GeometryCanvas*, Bucket b);

    virtual inline int make_buckets() { return 0; }
    virtual int execute(Bucket b) = 0;
    virtual inline int preprocess() { return 0; };
    virtual inline int postprocess() { return 0; };

    int launch();
};

#endif // BASE_TASK_HPP_INCLUDED
