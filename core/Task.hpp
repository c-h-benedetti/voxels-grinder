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

    virtual void undo();

    virtual ~Task() = default;
};

#endif // BASE_TASK_HPP_INCLUDED

/**
 * Cette classe devrait implémenter les mécanismes d'undo et redo au niveau des actions sur les données.
 */