#include "renderer.h"
#include "logging.h"
#include <glm/gtc/type_ptr.hpp>

using namespace renderlib;

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

const float vbo_data[] = {
	// x, y, z,
	// r, g, b, a
	-1.0f, -1.0f, 0.0f,
	1.0f, 0.0f, 0.0f, 1.0f,

	1.0f, -1.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 1.0f,

	0.0f,  1.0f, 0.0f,
	0.0f,  0.0f, 1.0f, 1.0f
};

GLuint vbo = 0;

void checkGLError(const char* op) {
    int error;
    while ((error = glGetError()) != GL_NO_ERROR) {
		LOG("glError (%s) : %d", op, error);
    }
}

void Renderer::init()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
    
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vbo_data), vbo_data, GL_STATIC_DRAW);
	checkGLError("init");
}

void Renderer::render(Camera& camera)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(simple_shader_.program_id());

	glm::mat4 mvp_mat = camera.get_projection_matrix() * camera.get_view_matrix();
	glUniformMatrix4fv(simple_shader_.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(mvp_mat));

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(simple_shader_.aPosition, 3, GL_FLOAT, GL_FALSE, 7 * 4, (void*)0);
	glVertexAttribPointer(simple_shader_.aColor, 4, GL_FLOAT, GL_FALSE, 7 * 4, (void*)(3 * 4));

	glDrawArrays(GL_TRIANGLES, 0, 3);
	checkGLError("render");
}
