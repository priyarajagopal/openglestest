#include "box.h"
#include <limits>

using namespace renderlib;

Box::Box(): 
	minpos(std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity()),
	maxpos(-std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity())
{
}

Box::Box(const glm::vec3& minp, const glm::vec3& maxp):
	minpos(minp), maxpos(maxp)
{
}

Box::Box(float xmin, float ymin, float zmin, float xmax, float ymax, float zmax):
	minpos(xmin, ymin, zmin), maxpos(xmax, ymax, zmax)
{
}

void Box::add_point(const glm::vec3& pos)
{
	minpos = glm::min(pos, minpos);
	maxpos = glm::max(pos, maxpos);
}

glm::vec3 Box::center() const
{
	return (minpos + maxpos) * 0.5f;
}

float Box::length() const
{
	return glm::distance(minpos, maxpos);
}