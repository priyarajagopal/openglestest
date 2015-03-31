#pragma once

#include "element_id.h"
#include "camera.h"

namespace renderlib {

class Renderer;
class ElementManager;

class Viewer 
{
public:
	Viewer();
	~Viewer();
	void init();
	void set_viewport(int x, int y, int w, int h);
	void draw();

	bool load_model_data(const char* data);

	const Camera& get_camera();
	void reset_camera();

	void set_elements_selected(const ElementIdList& ids, bool selected);
	void deselect_all_elements();
	ElementIdList get_selected_elements();
	
	void set_elements_visible(const ElementIdList& ids, bool visible);
	bool is_element_visible(const ElementId& id);

private:
	Camera camera;
	Renderer* renderer;
	ElementManager* element_manager;
	int viewport_x, viewport_y, viewport_w, viewport_h;
};

}