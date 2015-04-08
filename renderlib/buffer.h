#pragma once

#include <vector>
#include <memory>
#include "gles.h"
#include "parsed_objects.h"
#include "glm/glm.hpp"
#include "box.h"
#include "color.h"

namespace renderlib {

struct VertexStruct
{
	float position[3];
	float normal[3];
};

struct ColorStruct
{
	uint8_t color[4];
};

typedef uint32_t IndexType;

struct BufferOffset;

class Buffer
{
public:
	Buffer();
	~Buffer();

	bool add_ctm_data(CTMDecoder&  ctm, const glm::mat4& matrix, const RGBA& color, 
		Box& bbox, BufferOffset& offset);
	void load_buffers();
	void draw(GLint position_id, GLint normal_id, GLint color_id);
	bool is_loaded() const { return loaded; }
	void clear_buffers();
	bool is_transparent() const { return transparent; }
	void change_color(uint32_t vertex_offset, uint32_t vertex_count, const RGBA& color);

private:
	void unbind();
	void update_buffer_count();

	std::vector<VertexStruct> vertex_buffer;
	std::vector<ColorStruct> color_buffer;
	std::vector<IndexType> index_buffer;

	uint32_t buf_vertex_count;
	uint32_t buf_index_count;

	GLuint vertex_buffer_id;
	GLuint color_buffer_id;
	GLuint index_buffer_id;

	bool transparent;
	GLenum type;	// GL_TRIANGLES, GL_LINES
	bool loaded;
};

typedef std::shared_ptr<Buffer> BufferPtr;
typedef std::vector<BufferPtr> BufferPtrList;

struct BufferOffset
{
	BufferPtr buffer;
	uint32_t vertex_offset;
	uint32_t vertex_count;
	uint32_t index_offset;
	uint32_t index_count;
};

}

