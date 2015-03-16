#pragma once

#include "renderer.h"
#include "camera.h"

namespace renderlib {

class Viewer 
{
public:
	Viewer();
	~Viewer();
	void init();
	void set_viewport(int x, int y, int w, int h);
	void draw();

private:
	Camera camera_;
	Renderer renderer_;
	int viewport_x, viewport_y, viewport_w, viewport_h;
};

}