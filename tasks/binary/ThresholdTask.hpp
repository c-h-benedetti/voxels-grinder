#ifndef THRESHOLD_TASK_HPP_INCLUDED
#define THRESHOLD_TASK_HPP_INCLUDED

#include "Task.hpp"
#include "VoxelsCanvas.hpp"

class ThresholderTask : public Task {

public:

    enum class mode {
        AUTO,
        MANUAL
    };

    enum class method {
        OTSU,
        YEN,
        MEAN,
        INTERMODES
    };

private:

    VoxelsCanvas* target;
    VoxelsCanvas* output;
    float t1, t2;
    mode t_mode;
    method t_method;

public:

    ThresholderTask() = default;

    inline void useAutoThreshold(bool use) { t_mode = (use) ? (mode::AUTO) : (mode::MANUAL); }

};

#endif //THRESHOLD_TASK_HPP_INCLUDED