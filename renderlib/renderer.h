#pragma once

#include "shader_programs.h"
#include "camera.h"

namespace renderlib {

class Renderer
{
public:
	Renderer();
	~Renderer();
	void init();
	void render(Camera& camera);
private:
	SimpleShaderProgram simple_shader_;
};

}