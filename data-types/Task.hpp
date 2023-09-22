#ifndef BASE_DATA_TASK_HPP_INCLUDED
#define BASE_DATA_TASK_HPP_INCLUDED

#include <string>
#include <stdexcept>

#include "data-types.hpp"

struct BaseTask {

protected:

    std::string name;

protected:

    BaseTask() = delete;
    BaseTask(const std::string& n) : name(n) {}

public:

    virtual ~BaseTask() = default;

    virtual int runner(ImageMask&);
    virtual int runner(Image8&);
    virtual int runner(Image16&);
    virtual int runner(ImageFloat&);
    virtual int runner(ImageLabels&);
    virtual int runner(ImageRGB&);
};

#endif // BASE_DATA_TASK_HPP_INCLUDED