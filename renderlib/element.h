#pragma once

#include <string>
#include <vector>
#include <memory>
#include "element_id.h"
#include "color.h"
#include "geometry.h"

namespace renderlib {

class Element
{
public:
	Element(const ElementId& _element_id, uint8_t _type);
	~Element();

	ElementId element_id;
	bool selected;
	bool visible;
	uint8_t type;	// 1: space, 0: others

	GeometryPtrList geometries;
};

typedef std::shared_ptr<Element> ElementPtr;
typedef std::vector<ElementPtr> ElementPtrList;

}

