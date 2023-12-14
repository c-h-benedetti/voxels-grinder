#ifndef BASIC_PROCESSOR_HPP_INCLUDED
#define BASIC_PROCESSOR_HPP_INCLUDED

#include <string>

class Processor {

public:

    virtual void execute() = 0;
    virtual bool is_streamable() const = 0;
};

#endif // BASIC_PROCESSOR_HPP_INCLUDED