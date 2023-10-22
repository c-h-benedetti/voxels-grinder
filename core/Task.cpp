#include "Task.hpp"

int Task::run(const Data* a, Bucket b) {
    std::string s = "From " + this->get_name() + ": No runner implemented for type ";
    s += "Data.";
    throw std::unsupported_operation(s.c_str());
}


int Task::run(const MaskCanvas* a, Bucket b) {
    std::string s = "From " + this->get_name() + ": No runner implemented for type ";
    s += "MaskCanvas.";
    throw std::unsupported_operation(s.c_str());
}


int Task::run(const LabeledCanvas* a, Bucket b) {
    std::string s = "From " + this->get_name() + ": No runner implemented for type ";
    s += "LabeledCanvas.";
    throw std::unsupported_operation(s.c_str());
}


int Task::run(const VoxelsCanvasU8* a, Bucket b) {
    std::string s = "From " + this->get_name() + ": No runner implemented for type ";
    s += "VoxelsCanvasU8.";
    throw std::unsupported_operation(s.c_str());
}


int Task::run(const VoxelsCanvasU16* a, Bucket b) {
    std::string s = "From " + this->get_name() + ": No runner implemented for type ";
    s += "VoxelsCanvasU16.";
    throw std::unsupported_operation(s.c_str());
}


int Task::run(const VoxelsCanvasFloat* a, Bucket b) {
    std::string s = "From " + this->get_name() + ": No runner implemented for type ";
    s += "VoxelsCanvasFloat.";
    throw std::unsupported_operation(s.c_str());
}


int Task::run(const VoxelsCanvasTriplet* a, Bucket b) {
    std::string s = "From " + this->get_name() + ": No runner implemented for type ";
    s += "VoxelsCanvasTriplet.";
    throw std::unsupported_operation(s.c_str());
}


int Task::run(const Vertices* a, Bucket b) {
    std::string s = "From " + this->get_name() + ": No runner implemented for type ";
    s += "Vertices.";
    throw std::unsupported_operation(s.c_str());
}


int Task::run(const PolyLine* a, Bucket b) {
    std::string s = "From " + this->get_name() + ": No runner implemented for type ";
    s += "PolyLine.";
    throw std::unsupported_operation(s.c_str());
}


int Task::run(const Mesh* a, Bucket b) {
    std::string s = "From " + this->get_name() + ": No runner implemented for type ";
    s += "Mesh.";
    throw std::unsupported_operation(s.c_str());
}

