#ifndef GEOMETRY_HPP_INCLUDED
#define GEOMETRY_HPP_INCLUDED

#include "glm/glm.hpp"
#include "core/Data.hpp"

class Geometry {
	size_t channel;
	std::vector<glm::vec3> vertices;
	std::vector<glm::ivec3> indices;
	size_t id = 0;
};

class GeometryCanvas : public Data {
	// The first vector is for frames.
	// The second allows to have several geometric objects on each frame.
	std::vector<std::vector<Geometry>> geometries;

public:

	int run(Task* v, Bucket b) override;

	GeometryCanvas();
};

#endif // GEOMETRY_HPP_INCLUDED