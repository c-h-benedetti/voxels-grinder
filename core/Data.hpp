#ifndef DATA_ROOT_HPP_INCLUDED
#define DATA_ROOT_HPP_INCLUDED

#include <cstdint>
#include <memory>

class Task;
class DataProxy;

enum class DisplayFlags : uint64_t {
    mask   = 1 << 0,
    labels = 1 << 1
};

class Data {

public:

    /// Allows to summon the execution of a Task by this object (implements a visitor).
    virtual int run(Task* v, Bucket b) = 0;

protected:

    uint64_t flags = 0; // Only for display
    std::unique_ptr<DataProxy> proxy;

protected:

    /// Sets the proxy for this data, which allows to read and write from a file.
    inline void set_proxy(std::unique_ptr<DataProxy> p) { this->proxy = std::move(p); }

    virtual ~Data() = default;

    Data() = delete;
    Data(DataProxy* p): proxy(p) {}
};


class VoxelsCanvas;
class GeometryCanvas;

#endif //DATA_ROOT_HPP_INCLUDED

