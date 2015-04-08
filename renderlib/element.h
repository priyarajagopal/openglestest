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
	Element(const ElementId& _element_id, ElementType _type);
	~Element();
	void set_visible(bool visible);

	ElementId element_id;
	bool selected;
	bool visible;
	ElementType type;

	GeometryPtrList geometries;
};

typedef std::shared_ptr<Element> ElementPtr;
typedef std::vector<ElementPtr> ElementPtrList;

}

