#include "viewer.h"
#include "logging.h"
#include "renderer.h"
#include "camera.h"
#include "element_manager.h"
#include "box.h"

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
	element_manager->process_parsed_elements();
}

bool Viewer::load_model(const char* url)
{
	element_manager->load_model(url);
	return true;
}

void Viewer::fit_camera_to_box(float xmin, float ymin, float zmin, float xmax, float ymax, float zmax)
{
	camera.zoom_box(Box(xmin, ymin, zmin, xmax, ymax, zmax));
}