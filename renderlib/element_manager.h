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

	bool load_model_data(const char* data);

	std::map<ElementId, ElementPtr> elements;
	BufferPtrList buffers;
	Box bbox;

private:
	SharedGeometries shared_geoms;
	std::deque<ParsedElementPtr> parsed_elements;
	
	void processParsedElements();
	void addGeomToBuffer(BufferPtr& buffer, const ParsedGeometryPtr& geom, const glm::mat4& mat);
};

}

