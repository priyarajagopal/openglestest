#pragma once

#include <string>
#include <map>
#include <deque>
#include "element.h"
#include "parsed_objects.h"
#include "box.h"
#include <glm/glm.hpp>

namespace renderlib {

class ElementManager
{
public:
	ElementManager();
	~ElementManager();

	void load_model(const char* url);
	void process_parsed_elements();
	void add_shared_geom(const SharedGeomId& id, const ParsedGeometryPtrList& geometries);
	void queue_parse_element(ParsedElementPtr elem);
	void set_loading(bool);
	bool is_loading() const { return loading; }

	std::map<ElementId, ElementPtr> elements;
	BufferPtrList buffers;
	Box bbox;

private:
	SharedGeometries shared_geoms;
	std::deque<ParsedElementPtr> parsed_elements;
	BufferPtr current_opaque_buffer;
	BufferPtr current_transparent_buffer;
	bool loading;
	
	ParsedElementPtr get_next_parsed_element();
	BufferOffset add_geom_to_buffer(const ParsedGeometryPtr& geom, const glm::mat4& mat);
	BufferPtr* allocate_buffer(bool transparent);
	void thread_load_model(const std::string url);
};

}

