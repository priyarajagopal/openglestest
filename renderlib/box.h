#pragma once

#include <glm/glm.hpp>

namespace renderlib {

class Box
{
public:
	Box();
	Box(const glm::vec3& minp, const glm::vec3& maxp);
	void add_point(const glm::vec3& pos);
	glm::vec3 center() const;
	float length() const;

	glm::vec3 minpos;
	glm::vec3 maxpos;
};

}