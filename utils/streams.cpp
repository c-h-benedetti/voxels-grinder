#include "streams.hpp"

std::ostream& operator<<(std::ostream& os, const glm::ivec4& vec) {
    os << '(' << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << ')';
    return os;
}