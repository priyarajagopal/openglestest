#include "camera.h"
#include "glm/gtc/matrix_transform.hpp"

using namespace renderlib;

Camera::Camera(): 
	position(100, -100, 30), target(0, 0, 0), up(0, 0, 1),
	fov(45.0), aspect(1), near(0.1f), far(10000)
{
}

void Camera::set_aspect(int width, int height)
{
	aspect = ((float) width) / height;
}

glm::mat4 Camera::get_view_matrix() const
{
	return glm::lookAt(position, target, up);
}

glm::mat4 Camera::get_projection_matrix() const
{
	return glm::perspective(fov, aspect, near, far);
}

void Camera::zoom_box(const Box& box)
{
	glm::vec3 center = box.center();
	float distance = box.length() * 1.2f;
	glm::vec3 dir = position - target;
	dir = glm::normalize(dir);

	position = center + dir * distance;
	target = center;
}


