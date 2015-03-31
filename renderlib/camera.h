#pragma once

#include <glm/glm.hpp>
#include "box.h"

namespace renderlib {

class Camera
{
public:
	Camera();
	void set_aspect(int width, int height);
	glm::mat4 get_view_matrix() const;
	glm::mat4 get_projection_matrix() const;
	void zoom_box(const Box& box);

	glm::vec3 position;
	glm::vec3 target;
	glm::vec3 up;
	
	float fov;
	float aspect;
	float near;
	float far;
};

}