#include "renderer.h"
#include "logging.h"
#include "gles.h"
#include <glm/gtc/type_ptr.hpp>

using namespace renderlib;

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

void checkGLError(const char* op) {
    int error;
    while ((error = glGetError()) != GL_NO_ERROR) {
		LOG("glError (%s) : %d", op, error);
    }
}

void Renderer::init()
{
    glEnable(GL_DEPTH_TEST);
	//glEnable(GL_MULTISAMPLE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	//char* strExt = (char*) glGetString(GL_EXTENSIONS); 
	//LOG("Extensions : %s", strExt);

	glUseProgram(shader.program_id());

    glUniform3f(shader.uAmbientColor, 0.4f, 0.4f, 0.4f);
    glUniform3f(shader.uLight0Dir, 0.0f, 0.0f, 1.0f);
    glUniform3f(shader.uLight0Clr, 0.0f, 0.0f, 0.0f);

	glm::vec3 dir1(1,2,1);
	dir1 = glm::normalize(dir1);
    glUniform3f(shader.uLight1Dir, dir1.x, dir1.y, dir1.z);
    glUniform3f(shader.uLight1Clr, 0.5f, 0.5f, 0.5f);

	glm::vec3 dir2(-1,-1,7);
	dir2 = glm::normalize(dir2);
    glUniform3f(shader.uLight2Dir, dir2.x, dir2.y, dir2.z);
    glUniform3f(shader.uLight2Clr, 0.5f, 0.5f, 0.5f);
    
	checkGLError("init");
}

void Renderer::render(Camera& camera, ElementManager& element_manager)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glm::mat4 mvp_mat = camera.get_projection_matrix() * camera.get_view_matrix();
	glUniformMatrix4fv(shader.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(mvp_mat));

	// render opaque
	for (BufferPtr& buffer : element_manager.buffers)
	{
		if (!buffer->is_transparent())
			buffer->draw(shader.aPosition, shader.aNormal, shader.aColor);
	}

	// render transparent
	glEnable(GL_BLEND);
	glDepthMask(GL_FALSE);
	for (BufferPtr& buffer : element_manager.buffers)
	{
		if (buffer->is_transparent())
			buffer->draw(shader.aPosition, shader.aNormal, shader.aColor);
	}
    glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);

	checkGLError("render");
}
