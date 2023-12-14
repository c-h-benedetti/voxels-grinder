#include <cstring>
#include "containers/VoxelsCanvas.hpp"
#include "core/Task.hpp"

VoxelsCanvas::VoxelsCanvas(DataProxy* p) : Data(p) {

}


VoxelsCanvas::VoxelsCanvas(Bucket b) : Data(nullptr) {
    if ((b.nVoxelsX() == 0) || 
        (b.nVoxelsY() == 0) || 
        (b.nVoxelsZ() == 0) || 
        (b.nFrames()  == 0)) { 
			throw std::invalid_argument("No dimension should be 0.");
	}
    this->global = b;
    this->allocate_memory(b);
}


VoxelsCanvas::VoxelsCanvas(size_t h, size_t w, size_t s, size_t f) : 
    VoxelsCanvas(Bucket({0, h}, {0, w}, {0, s}, {0, f})) {}


// Has to deal with stream creation if the required canvas is too big.
bool VoxelsCanvas::allocate_memory(Bucket b) {
	this->data = new float [b.length()];
	return true;
}

/// Creates a copy from the original VoxelsCanvas instance.
VoxelsCanvas::VoxelsCanvas(const VoxelsCanvas& vc) : VoxelsCanvas(vc.get_global_dimensions()) {
	memcpy(this->data, vc.data, this->global.length() * sizeof(float));
}

/// Creates a copy from a given region of a VoxelsCanvas instance.
VoxelsCanvas::VoxelsCanvas(const VoxelsCanvas& vc, const Bucket& b) : Data(nullptr) {

}

void VoxelsCanvas::to_dump(const std::filesystem::path& p) {

}


void VoxelsCanvas::from_dump(const std::filesystem::path& p) {

}


float& VoxelsCanvas::at(size_t c, size_t l, size_t s, size_t f) {
	return this->data[this->loaded.index_of(c, l, s, f)];
}


void  VoxelsCanvas::set(size_t c, size_t l, size_t s, size_t f, float val) {
	this->data[this->loaded.index_of(c, l, s, f)] = val;
}


int VoxelsCanvas::run(Task* v, Bucket b) {
	return v->run(this, b);
}


/** TODO:
 * - [ ] Remove the `Data(nullptr)` call where a file should be used instead.
 * - [ ] Have an option in VG that allows to delete temporary files when we close the software.
 * - [ ] Have an option to show bounding-boxes of images when we load something.
 *       Also a way to see the loading areas.
 */