#include <cstring>
#include "containers/VoxelsCanvas.hpp"
#include "core/Task.hpp"

VoxelsCanvas::VoxelsCanvas(DataProxy* p) : Data(p) {

}


VoxelsCanvas::VoxelsCanvas(Bucket b) : Data(nullptr), dimensions(b) {
    this->allocate_memory(b);
}


VoxelsCanvas::VoxelsCanvas(size_t w, size_t h, size_t s, size_t f) : 
    VoxelsCanvas(Bucket(w, h, s, f)) {}


VoxelsCanvas::~VoxelsCanvas() {
	delete[] this->data;
	this->data = nullptr;
}


// Has to deal with stream creation if the required canvas is too big.
bool VoxelsCanvas::allocate_memory(Bucket b) {
	this->data = new float [b.get_canvas_size()];
	return true;
}

/// Creates a copy from the original VoxelsCanvas instance.
VoxelsCanvas::VoxelsCanvas(const VoxelsCanvas& vc) : VoxelsCanvas(vc.get_dimensions()) {
	memcpy(this->data, vc.data, this->dimensions.get_canvas_size() * sizeof(float));
}

/// Creates a copy from a given region of a VoxelsCanvas instance.
VoxelsCanvas::VoxelsCanvas(const VoxelsCanvas& vc, const Bucket& b) : Data(nullptr) {
	Bucket new_bucket(b.get_local_columns(), b.get_local_rows(), b.get_local_slices(), b.get_local_frames());
	new_bucket.set_start_frame(b.get_start_frame() + b.get_frames_range().first);
	
	glm::vec3 o = b.get_origin();
	Calibration c = b.get_calibration();
	new_bucket.set_calibration(c);
	
	o.x += static_cast<float>(b.get_columns_range().first) * c.get_size_x();
	o.y += static_cast<float>(b.get_rows_range().first) * c.get_size_y();
	o.z += static_cast<float>(b.get_slices_range().first) * c.get_size_z();

	new_bucket.set_origin(o);
	this->dimensions = new_bucket;

	this->allocate_memory(new_bucket);
	Bucket::Iterator it = this->dimensions.get_iterator();
	size_t pos = 0;

	while (it) {
		this->data[pos++] = vc.at(it);
        it.next();
	}
}


float VoxelsCanvas::at(size_t c, size_t l, size_t s, size_t f) const {
	return this->data[this->dimensions.index_of(c, l, s, f)];
}


float VoxelsCanvas::at(const Bucket::Iterator& it) const {
	return this->data[it.index_of()];
}


void  VoxelsCanvas::set(size_t c, size_t l, size_t s, size_t f, float val) {
	this->data[this->dimensions.index_of(c, l, s, f)] = val;
}


void VoxelsCanvas::set(const Bucket::Iterator& it, float val) {
	this->data[it.index_of()] = val;
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