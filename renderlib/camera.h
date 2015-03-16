#pragma once

#include <glm/glm.hpp>

namespace renderlib {

class Camera
{
public:
	Camera();
	void set_aspect(int width, int height);
	glm::mat4 get_view_matrix();
	glm::mat4 get_projection_matrix();

	glm::vec3 position;
	glm::vec3 target;
	glm::vec3 up;
	
	float fov;
	float aspect;
	float near;
	float far;
};

}