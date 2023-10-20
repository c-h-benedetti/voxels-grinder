#include "Task.hpp"
#include <stdexcept>

int Task::run(const Data*) {
    std::string s = "From " + this->get_name() + ": No runner implemented for type ";
    s += "Data.";
    throw std::invalid_argument(s.c_str());
}


int Task::run(const MaskCanvas*) {
    std::string s = "From " + this->get_name() + ": No runner implemented for type ";
    s += "MaskCanvas.";
    throw std::invalid_argument(s.c_str());
}


int Task::run(const LabeledCanvas*) {
    std::string s = "From " + this->get_name() + ": No runner implemented for type ";
    s += "LabeledCanvas.";
    throw std::invalid_argument(s.c_str());
}


int Task::run(const VoxelsCanvasU8*) {
    std::string s = "From " + this->get_name() + ": No runner implemented for type ";
    s += "VoxelsCanvasU8.";
    throw std::invalid_argument(s.c_str());
}


int Task::run(const VoxelsCanvasU16*) {
    std::string s = "From " + this->get_name() + ": No runner implemented for type ";
    s += "VoxelsCanvasU16.";
    throw std::invalid_argument(s.c_str());
}


int Task::run(const VoxelsCanvasFloat*) {
    std::string s = "From " + this->get_name() + ": No runner implemented for type ";
    s += "VoxelsCanvasFloat.";
    throw std::invalid_argument(s.c_str());
}


int Task::run(const VoxelsCanvasTriplet*) {
    std::string s = "From " + this->get_name() + ": No runner implemented for type ";
    s += "VoxelsCanvasTriplet.";
    throw std::invalid_argument(s.c_str());
}


int Task::run(const Vertices*) {
    std::string s = "From " + this->get_name() + ": No runner implemented for type ";
    s += "Vertices.";
    throw std::invalid_argument(s.c_str());
}


int Task::run(const PolyLine*) {
    std::string s = "From " + this->get_name() + ": No runner implemented for type ";
    s += "PolyLine.";
    throw std::invalid_argument(s.c_str());
}


int Task::run(const Mesh*) {
    std::string s = "From " + this->get_name() + ": No runner implemented for type ";
    s += "Mesh.";
    throw std::invalid_argument(s.c_str());
}

