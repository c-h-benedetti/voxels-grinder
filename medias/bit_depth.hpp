#ifndef BIT_DEPTH_HPP_INCLUDED
#define BIT_DEPTH_HPP_INCLUDED

#include <cinttypes>

enum bit_depth: uint8_t {
    BYTE  = 8,
    SHORT = 16,
    FLOAT = 32,
    RGB   = 24
};

#endif // BIT_DEPTH_HPP_INCLUDED