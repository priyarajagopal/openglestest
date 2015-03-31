#pragma once

#include <vector>
#include "color.h"
#include "buffer.h"

namespace renderlib {

struct BufferRef
{
	BufferPtr buffer;
	uint32_t vertex_offset;
	size_t vertex_count;
	uint32_t index_offset;
	size_t index_count;
};

class Geometry
{
public:
	Geometry();
	~Geometry();

	RGBA color;
	uint8_t type;	// 0: triangles, 1: lines

	BufferRef buffer_ref;
};

typedef std::shared_ptr<Geometry> GeometryPtr;
typedef std::vector<GeometryPtr> GeometryPtrList;

}

