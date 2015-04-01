#pragma once

#include "shader_programs.h"
#include "camera.h"
#include "element_manager.h"

namespace renderlib {

class Renderer
{
public:
	Renderer();
	~Renderer();
	void init();
	void render(Camera& camera, ElementManager& element_manager);
private:
	//SimpleShaderProgram shader_;
	RegularShaderProgram shader;
};

}