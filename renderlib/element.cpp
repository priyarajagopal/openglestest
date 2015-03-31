#include "element.h"

using namespace renderlib;

Element::Element(const char* _element_id, uint8_t _type):
	element_id(_element_id),
	selected(false), visible(true),
	type(_type)
{
}


Element::~Element()
{
}

