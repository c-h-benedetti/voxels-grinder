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
    virtual int run(const VoxelsCanvas*, Bucket b);
    virtual int run(const GeometryCanvas*, Bucket b);

    virtual int execute(Bucket b) = 0;
};

#endif // BASE_TASK_HPP_INCLUDED
