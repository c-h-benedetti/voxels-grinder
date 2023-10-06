#ifndef DATA_ROOT_HPP_INCLUDED
#define DATA_ROOT_HPP_INCLUDED

#include <vector>
#include "Bucket.hpp"

class Task;

class Data {

    std::vector<Bucket> buckets;

public:

    inline const std::vector<Bucket>& get_buckets() { return buckets; }

    virtual int run(Task& v) = 0;
    virtual Data* alike() = 0;

    virtual ~Data() = default;

    Data() = default;
};

class VoxelsCanvas;
class Vertices;
class PolyLine;
class Mesh;

#endif //DATA_ROOT_HPP_INCLUDED