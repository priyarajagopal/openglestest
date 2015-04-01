#pragma once

#include <vector>
#include <memory>
#include "color.h"
#include "buffer.h"

namespace renderlib {

class Geometry
{
public:
	Geometry(const RGBA& color_, uint8_t type_);
	~Geometry();

	RGBA color;
	uint8_t type;	// 0: triangles, 1: lines

	BufferOffset buffer_offset;
};

typedef std::shared_ptr<Geometry> GeometryPtr;
typedef std::vector<GeometryPtr> GeometryPtrList;

}

