#ifndef ROOT_DATA_TYPE_HPP_INCLUDED
#define ROOT_DATA_TYPE_HPP_INCLUDED

#include "Task.hpp"

class DataContainer {
    virtual int run(BaseTask& v) = 0;
};

#endif // ROOT_DATA_TYPE_HPP_INCLUDED