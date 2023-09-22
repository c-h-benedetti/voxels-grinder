#ifndef IMAGE_BASE_CLASS_INCLUDED
#define IMAGE_BASE_CLASS_INCLUDED

#include "Task.hpp"
#include "Data.hpp"

template <typename T>
class Image : public DataContainer {

public:

    Image() = default;

private:

    int run(BaseTask& v) override;
};


// +----------------------------------------------------------+
// |  DEFINITIONS                                             |
// +----------------------------------------------------------+


template <typename T>
int Image<T>::run(BaseTask& v) {
    return v.runner(*this);
}


#endif // IMAGE_BASE_CLASS_INCLUDED