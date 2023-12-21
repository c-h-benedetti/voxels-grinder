#include "gtest/gtest.h"
#include "containers/VoxelsCanvas.hpp"

#include <random>
#include <cstring>

// # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
// #    VOXELS CANVAS                                                          #
// # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

// Dimensions used for testing purpose
constexpr size_t width  = 512;
constexpr size_t height = 512;
constexpr size_t depth  = 15;
constexpr size_t frames = 7;

// >>> Constructors:

TEST(VoxelsCanvas, VoxelsCanvasDimensionsConstructor) {
    RecordProperty("short", "Instanciate a canvas from dimensions.");
 
    EXPECT_NO_THROW(VoxelsCanvas vc = VoxelsCanvas(width, height, depth, frames););
}


TEST(VoxelsCanvas, VoxelsCanvasBucketConstructor) {
    RecordProperty("short", "Instanciate a canvas from dimensions.");

    Bucket b1(width, height, depth, frames);
    EXPECT_NO_THROW(VoxelsCanvas vc = VoxelsCanvas(b1););
}


TEST(VoxelsCanvas, VoxelsCanvasFullCopyConstructor) {
    RecordProperty("short", "Instanciate a canvas from another canvas.");

    Bucket b1(width, height, depth, frames);
    VoxelsCanvas vc2(b1);
    EXPECT_NO_THROW(VoxelsCanvas vc3(vc2););
}


std::pair<size_t, size_t> mid(size_t dim) {
    size_t q = dim / 3;
    return {q, 2*q};
}


TEST(VoxelsCanvas, VoxelsCanvasPartialCopyConstructor) {
    RecordProperty("short", "Instanciate a canvas from another canvas.");

    Bucket b1(width, height, depth, frames);
    VoxelsCanvas vc2(b1);
    
    Bucket b2(
        b1,
        mid(width),
        mid(height),
        mid(depth),
        mid(frames)
    );

    EXPECT_NO_THROW(VoxelsCanvas vc4(vc2, b2););
}

// >>> Data validity

TEST(VoxelsCanvas, VoxelsCanvasFullCopyValidity) {
    RecordProperty("short", "Instanciate a canvas from another canvas.");

    Bucket b1(width, height, depth, frames);
    VoxelsCanvas vc2(b1);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    Bucket::Iterator it = b1.get_iterator();

    while (it) {
        vc2.set(it, dis(gen));
        it.next();
    }

    VoxelsCanvas vc3(vc2);

    EXPECT_TRUE(memcmp(
        vc2.get_data_segment(), 
        vc3.get_data_segment(), 
        sizeof(float)*b1.get_canvas_size()
    ) == 0);
}


TEST(VoxelsCanvas, VoxelsCanvasPartialCopyValidity) {
    RecordProperty("short", "Instanciate a canvas from another canvas.");

    Bucket b1(width, height, depth, frames);
    VoxelsCanvas vc2(b1);
    
    Bucket b2(
        b1,
        mid(width),
        mid(height),
        mid(depth),
        mid(frames)
    );

    VoxelsCanvas vc4(vc2, b2);
}