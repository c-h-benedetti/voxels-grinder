#include "VoxelsCanvas.hpp"
#include "Task.hpp"

VoxelsCanvas::VoxelsCanvas(DataProxy* p) : Data(p) {

}


VoxelsCanvas::VoxelsCanvas(Bucket b) : Data(nullptr) {
    if ((b.nVoxelsX() == 0) || 
        (b.nVoxelsY() == 0) || 
        (b.nVoxelsZ() == 0) || 
        (b.nFrames() == 0)) { throw std::invalid_argument("No dimension should be 0."); }
    
    this->global = b;
    this->allocate_memory(b);
}


int VoxelsCanvas::run(Task* v, Bucket b) {
	return v->run(this, b);
}


VoxelsCanvas::VoxelsCanvas(size_t h, size_t w, size_t s, size_t f) : 
    VoxelsCanvas(Bucket({0, h}, {0, w}, {0, s}, {0, f})) {}


// Has to deal with stream creation if the required canvas is too big.
bool VoxelsCanvas::allocate_memory(Bucket b) {
	float* raw = nullptr;

	try {
		raw = new float[b.length()];
		if (raw == nullptr) { throw std::invalid_argument("Canvas is too big, allocation failed, need a stream."); }
		this->loaded = b;
		return true; // We successfuly allocated the memory
	} catch (std::exception e) {
		raw = nullptr;
		return false;
	}

	// Steps to create a stream:
	//   1. Cut into buckets (buckets must have all the slices).
	//   2. Create dumps (we must attribute IDs and write 0s on the disk).
	//   3. Prepare borders.
	//   4. Prepare the pyramids.
	return true;
}