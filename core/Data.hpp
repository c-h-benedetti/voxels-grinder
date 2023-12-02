#ifndef DATA_ROOT_HPP_INCLUDED
#define DATA_ROOT_HPP_INCLUDED

#include <cstdint>
#include <memory>
#include "Bucket.hpp"
#include "DataProxy.hpp"

class Task;
class DataProxy;


class Data {

public:

    // Allows to summon the execution of a Task by this object (implements a visitor).
    virtual int run(Task* v, Bucket b) = 0;
    virtual ~Data() = default;

protected:

    // Used to access the dataproxy for the opened file. All channels share the same proxy.
    std::shared_ptr<DataProxy> proxy;

protected:

    // Sets the proxy for this data, which allows to read and write from a file.
    inline void set_proxy(std::shared_ptr<DataProxy> p) { this->proxy = p; }

    Data() = delete;
    Data(DataProxy* p): proxy(p) {}
};


class VoxelsCanvas;
class GeometryCanvas;

#endif //DATA_ROOT_HPP_INCLUDED

