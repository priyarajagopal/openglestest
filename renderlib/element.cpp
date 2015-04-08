#include "element.h"

using namespace renderlib;

Element::Element(const ElementId& _element_id, ElementType _type):
	element_id(_element_id),
	selected(false), visible(true),
	type(_type)
{
}


Element::~Element()
{
	geometries.clear();
}

void Element::set_visible(bool visible_)
{
	if (visible_ == visible) return;
	visible = visible_;
	for (auto& geom : geometries)
	{
		if (visible)
			geom->reset_color();
		else
			geom->change_color(RGBA(0,0,0,0));
	}
}

