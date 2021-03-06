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
	element_manager = new ElementManager(this);
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
	//LOG("set_viewport %d %d %d %d", x, y, w, h);
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

void Viewer::set_elements_visible(const ElementIdList& ids, bool visible)
{
	for (auto& id : ids)
	{
		ElementPtr element = element_manager->get_element_by_id(id);
		if (element)
			element->set_visible(visible);
	}
}

bool Viewer::is_element_visible(const ElementId& id)
{
	ElementPtr element = element_manager->get_element_by_id(id);
	return element ? element->visible : false;
}