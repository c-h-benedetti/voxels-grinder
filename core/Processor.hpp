#ifndef BASIC_PROCESSOR_HPP_INCLUDED
#define BASIC_PROCESSOR_HPP_INCLUDED

#include <string>

class Processor {

public:

    virtual void undo() = 0;
    virtual std::wstring get_name() const = 0;
    virtual std::string get_identifier() const = 0;
    virtual bool is_streamable() const = 0;
};

#endif // BASIC_PROCESSOR_HPP_INCLUDED