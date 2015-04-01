#include "element.h"

using namespace renderlib;

Element::Element(const ElementId& _element_id, uint8_t _type):
	element_id(_element_id),
	selected(false), visible(true),
	type(_type)
{
}


Element::~Element()
{
	geometries.clear();
}

