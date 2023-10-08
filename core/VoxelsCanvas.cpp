#include "VoxelsCanvas.hpp"


void VoxelsCanvas::require(glm::ivec3 size, padding_type type) {

}


void VoxelsCanvas::require(glm::vec3 size, padding_type type) {

}


VoxelsCanvas::VoxelsCanvas* alike() const {

}


VoxelsCanvas::VoxelsCanvas(size_t height, size_t width, size_t nSlices, size_t nChannels, size_t nFrames) {

}


VoxelsCanvas::~VoxelsCanvas() {

}

/*
Only the loaded part must be turned into buckets. Its useless to summon a processing function over a chunk that's not loaded.
It implies that we need another function that turns a global Bucket into loading buckets.
*/
std::vector<Bucket> bucketize(const VoxelsCanvas* vc) {

}

