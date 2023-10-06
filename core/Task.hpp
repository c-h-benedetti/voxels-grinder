#ifndef BASE_TASK_HPP_INCLUDED
#define BASE_TASK_HPP_INCLUDED

#include <string>
#include <stdexcept>
#include "Data.hpp"

struct Task {

protected:

    std::string name;
    
    Task() = delete;
    Task(const std::string& n) : name(n) {}

public:
    virtual ~Task() = default;

    virtual int runner(VoxelsCanvas&) {
        std::string m1 = "Operation `";
        std::string m2 = "` is not available for type VoxelsCanvas.";
        std::string m  = m1 + name + m2;
        throw std::runtime_error(m.c_str());
    }
};

#endif // BASE_TASK_HPP_INCLUDED