#include "geometry.h"

using namespace renderlib;

Geometry::Geometry(const RGBA& color_, uint8_t type_):
	color(color_), type(type_)
{
}

Geometry::~Geometry()
{
}

