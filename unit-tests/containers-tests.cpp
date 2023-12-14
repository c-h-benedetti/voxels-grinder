#include "gtest/gtest.h"
#include "containers/VoxelsCanvas.hpp"

// # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
// #    BEHAVIOR TESTS                                                         #
// # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

TEST(CoreBehavior, CanvasInstanciation) {
    RecordProperty("description", "How to instanciate (and access) a VoxelsCanvas object that fits in RAM.");

    // Dimensions used for testing purpose
    constexpr size_t height = 512;
    constexpr size_t width  = 512;
    constexpr size_t depth  = 34;
    constexpr size_t frames = 15;

    // Instanciate from dimensions
    VoxelsCanvas vc1(height, width, depth, frames);

    // Instanciate from a Bucket (reprensenting dimensions)
    Bucket b1(height, width, depth, frames);
    VoxelsCanvas vc2(b1);

    // Instanciante from another instance.
    VoxelsCanvas vc3(vc2);

    // Instanciante from a chunk from another instance.
    Bucket b2(height/2, width/2, depth/2, frames/2);
    VoxelsCanvas vc3(vc1, b2);

    // Instanciate from a raw-dump
    VoxelsCanvas vc4("/home/clement/tests/dump.rawdump");
}