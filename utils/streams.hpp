#ifndef STREAMS_UTILS_HPP_INCLUDED
#define STREAMS_UTILS_HPP_INCLUDED

#include "glm/glm.hpp"
#include <iostream>

std::ostream& operator<<(std::ostream& os, const glm::ivec4& vec);

#endif // STREAMS_UTILS_HPP_INCLUDED