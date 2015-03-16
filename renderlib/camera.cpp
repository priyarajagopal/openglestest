#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>

using namespace renderlib;

Camera::Camera(): 
	position(4, 3, 3), target(0, 0, 0), up(0, 1, 0),
	fov(45), aspect(1), near(0.1), far(100)
{
}

void Camera::set_aspect(int width, int height)
{
	aspect = ((float) width) / height;
}

glm::mat4 Camera::get_view_matrix()
{
	return glm::lookAt(position, target, up);
}

glm::mat4 Camera::get_projection_matrix()
{
	return glm::perspective(fov, aspect, near, far);
}


