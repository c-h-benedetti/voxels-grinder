#include "GeometryCanvas.hpp"
#include "core/Task.hpp"

int GeometryCanvas::run(Task* v, Bucket b) {
	return v->run(this, b);
}


GeometryCanvas::GeometryCanvas() : Data(nullptr) {}