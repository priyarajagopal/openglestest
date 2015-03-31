#pragma once

#include "openctm.h"

namespace renderlib {

class CTMDecoder
{
public:
	CTMDecoder();
	~CTMDecoder();

	void decode(const char* data);
	
	unsigned int get_vertex_count();
	unsigned int get_tri_count();

	const float* get_vertices();
	const unsigned int* get_indices();

private:
	CTMcontext ctmCtx;
};

}