#include "geometry.h"

using namespace renderlib;

Geometry::Geometry(const RGBA& color_, uint8_t type_):
	color(color_), type(type_)
{
}

Geometry::~Geometry()
{
}

void Geometry::change_color(const RGBA& color_)
{
	buffer_offset.buffer->change_color(buffer_offset.vertex_offset, 
		buffer_offset.vertex_count, color_);
}

void Geometry::reset_color()
{
	buffer_offset.buffer->change_color(buffer_offset.vertex_offset, 
		buffer_offset.vertex_count, color);
}
