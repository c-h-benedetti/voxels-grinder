#include "Task.hpp"

int BaseTask::runner(ImageMask&) {
    std::string m1 = "Operation `";
    std::string m2 = "` is not available for type ImageMask.";
    std::string m  = m1 + name + m2;
    throw std::runtime_error(m.c_str());
}

int BaseTask::runner(Image8&) {
    std::string m1 = "Operation `";
    std::string m2 = "` is not available for type Image8.";
    std::string m  = m1 + name + m2;
    throw std::runtime_error(m.c_str());
}

int BaseTask::runner(Image16&) {
    std::string m1 = "Operation `";
    std::string m2 = "` is not available for type Image16.";
    std::string m  = m1 + name + m2;
    throw std::runtime_error(m.c_str());
}

int BaseTask::runner(ImageFloat&) {
    std::string m1 = "Operation `";
    std::string m2 = "` is not available for type ImageFloat.";
    std::string m  = m1 + name + m2;
    throw std::runtime_error(m.c_str());
}

int BaseTask::runner(ImageLabels&) {
    std::string m1 = "Operation `";
    std::string m2 = "` is not available for type ImageLabels.";
    std::string m  = m1 + name + m2;
    throw std::runtime_error(m.c_str());
}

int BaseTask::runner(ImageRGB&) {
    std::string m1 = "Operation `";
    std::string m2 = "` is not available for type ImageRGB.";
    std::string m  = m1 + name + m2;
    throw std::runtime_error(m.c_str());
}
