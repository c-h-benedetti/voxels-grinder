#include "VoxelsCanvas.hpp"

VoxelsCanvas::VoxelsCanvas(DataProxy* p) {

}


VoxelsCanvas::VoxelsCanvas(Bucket b) {
    if ((b.height() == 0) || 
        (b.width() == 0) || 
        (b.nSlices() == 0) || 
        (b.nChannels() == 0) || 
        (b.nFrames() == 0)) { throw std::invalid_argument("No dimension should be 0."); }
    
    this->global = b;
    this->allocate_memory();
}


VoxelsCanvas::VoxelsCanvas(size_t h, size_t w, size_t s, size_t c, size_t f) : 
    VoxelsCanvas(Bucket({0, h}, {0, w}, {0, s}, {0, c}, {0, f})) {}


// Has to deal with stream creation if the required canvas is too big.
int VoxelsCanvas::allocate_memory(Bucket b) {
	c_float raw = nullptr;

	try {
		raw = new c_float[b.length()];
		if (raw == nullptr) { throw std::invalid_argument("Canvas is too big, allocation failed, need a stream."); }
		this->local = b;
		return; // We successfuly allocated the memory
	} catch (std::exception e) {
		raw = nullptr;
	}

	// Steps to create a stream:
	//   1. Cut into buckets (buckets must have all the slices).
	//   2. Create dumps (we must attribute IDs and write 0s on the disk).
	//   3. Prepare borders.
	//   4. Prepare the pyramids.
}