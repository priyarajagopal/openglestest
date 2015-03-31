#include "viewer.h"
#include "logging.h"
#include "renderer.h"
#include "camera.h"
#include "element_manager.h"

#include "SampleHouse.h"

using namespace renderlib;

Viewer::Viewer()
{
	renderer = new Renderer();
	element_manager = new ElementManager();
}


Viewer::~Viewer()
{
	delete renderer;
	delete element_manager;
}

void Viewer::init()
{
	LOGW("init");
	renderer->init();
	
	load_model_data(sample_house_json().c_str());
}

void Viewer::set_viewport(int x, int y, int w, int h)
{
	LOG("set_viewport %d %d %d %d", x, y, w, h);
	viewport_x = x;
	viewport_y = y;
	viewport_w = w;
	viewport_h = h;
	glViewport(viewport_x, viewport_y, viewport_w, viewport_h);
	camera.set_aspect(viewport_w, viewport_h);
}

void Viewer::draw()
{
	renderer->render(camera, *element_manager);
}

bool Viewer::load_model_data(const char* data)
{
	element_manager->load_model_data(data);
	camera.zoom_box(element_manager->bbox);
	return true;
}