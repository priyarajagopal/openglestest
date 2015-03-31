#include <string>
#include "ctm_decoder.h"
#include "base64.h"

using namespace renderlib;

CTMDecoder::CTMDecoder():
	ctmCtx(nullptr)
{
	
}

CTMDecoder::~CTMDecoder()
{
	if (ctmCtx)
		ctmFreeContext(ctmCtx);
}

struct octm_buffer
{
	char* data;
	unsigned int current;

	octm_buffer(size_t len) : data(new char[len]), current(0) {};
	
	~octm_buffer()
	{
		delete [] data;
	}
};

static CTMuint readfn(void * aBuf, CTMuint aCount, void * aUserData)
{
	octm_buffer* buffer = (octm_buffer*) aUserData;
	memcpy(aBuf, buffer->data + buffer->current, aCount);
	buffer->current += aCount;
	return aCount;
}

void CTMDecoder::decode(const char* data)
{
	ctmCtx = ctmNewContext(CTM_IMPORT);
	int datalen = Base64decode_len(data);
	octm_buffer buffer(datalen);
	Base64decode(buffer.data, data);
	ctmLoadCustom(ctmCtx, readfn, &buffer);
}

unsigned int CTMDecoder::get_tri_count()
{
	return ctmGetInteger(ctmCtx, CTM_TRIANGLE_COUNT);
}

unsigned int CTMDecoder::get_vertex_count()
{
	return ctmGetInteger(ctmCtx, CTM_VERTEX_COUNT);
}

const unsigned int* CTMDecoder::get_indices()
{
	return ctmGetIntegerArray(ctmCtx, CTM_INDICES);
}

const float* CTMDecoder::get_vertices()
{
	return ctmGetFloatArray(ctmCtx, CTM_VERTICES);
}