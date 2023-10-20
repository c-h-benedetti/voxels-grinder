#ifndef BASE_TASK_HPP_INCLUDED
#define BASE_TASK_HPP_INCLUDED

#include <string>
#include <stdexcept>
#include "Data.hpp"

class Task {

protected:
    
    Task() = default;

    virtual int run(const Data*);

    virtual int run(const MaskCanvas*);
    virtual int run(const LabeledCanvas*);

    virtual int run(const VoxelsCanvasU8*);
    virtual int run(const VoxelsCanvasU16*);
    virtual int run(const VoxelsCanvasFloat*);
    virtual int run(const VoxelsCanvasTriplet*);

    virtual int run(const Vertices*);
    virtual int run(const PolyLine*);
    virtual int run(const Mesh*);

public:

    virtual const std:: string get_name() const = 0;

    virtual bool execute() = 0;
    virtual void undo();

    virtual ~Task() = default;

    friend class Data;
};

#endif // BASE_TASK_HPP_INCLUDED
