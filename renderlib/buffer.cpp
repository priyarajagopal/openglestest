#include "buffer.h"
#include "glm/glm.hpp"

using namespace renderlib;

#define MAX_VERTEX_COUNT 150000
#define MAX_INDEX_COUNT 600000

Buffer::Buffer():
	vertex_buffer_id(0), color_buffer_id(0), index_buffer_id(0),
	buf_vertex_count(0), buf_index_count(0),
	transparent(false), type(GL_TRIANGLES),
	loaded(false)
{
}

Buffer::~Buffer()
{
	clear_buffers();
	unbind();
	GLuint buffers[] = {vertex_buffer_id, color_buffer_id, index_buffer_id};
	glDeleteBuffers(3, buffers);
}

void Buffer::unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Buffer::update_buffer_count()
{
	buf_vertex_count = vertex_buffer.size();
	buf_index_count = index_buffer.size();
}

bool Buffer::add_ctm_data(CTMDecoder& ctm, const glm::mat4& matrix, const RGBA& color, Box& bbox, BufferOffset& offset)
{
	auto vertCnt = ctm.get_vertex_count();
	auto triCnt = ctm.get_tri_count();
	auto indexCnt = triCnt * 3;

	update_buffer_count();

	if ((buf_vertex_count + vertCnt) > MAX_VERTEX_COUNT || (buf_index_count + indexCnt) > MAX_INDEX_COUNT)
		return false;

	vertex_buffer.reserve(buf_vertex_count + vertCnt);
	index_buffer.reserve(buf_index_count + indexCnt);

	transparent = color.is_transparent();

	const float* ctm_vertices = ctm.get_vertices();
	const unsigned int* ctm_indices = ctm.get_indices();

	offset.vertex_offset = buf_vertex_count;
	offset.vertex_count = vertCnt;
	offset.index_offset = buf_index_count;
	offset.index_count = indexCnt;

	for (unsigned int i=0; i < vertCnt; i++)
	{
		glm::vec4 pos(ctm_vertices[i * 3], ctm_vertices[i * 3 + 1], ctm_vertices[i * 3 + 2], 1);
		pos = matrix * pos;
		bbox.add_point(glm::vec3(pos));

		VertexStruct vertex;
		vertex.position[0] = pos.x;
		vertex.position[1] = pos.y;
		vertex.position[2] = pos.z;

		vertex.normal[0] = 0;
		vertex.normal[1] = 0;
		vertex.normal[2] = 0;

		ColorStruct clr;
		clr.color[0] = color.r();
		clr.color[1] = color.g();
		clr.color[2] = color.b();
		clr.color[3] = color.a();

		vertex_buffer.push_back(vertex);
		color_buffer.push_back(clr);
	}

	for (unsigned int i=0; i < indexCnt; i += 3)
	{
		auto i0 = ctm_indices[i] + buf_vertex_count;
		auto i1 = ctm_indices[i + 1] + buf_vertex_count;
		auto i2 = ctm_indices[i + 2] + buf_vertex_count;

		index_buffer.push_back(i0);
		index_buffer.push_back(i1);
		index_buffer.push_back(i2);

		glm::vec3 p0(vertex_buffer[i0].position[0], vertex_buffer[i0].position[1], vertex_buffer[i0].position[2]);
		glm::vec3 p1(vertex_buffer[i1].position[0], vertex_buffer[i1].position[1], vertex_buffer[i1].position[2]);
		glm::vec3 p2(vertex_buffer[i2].position[0], vertex_buffer[i2].position[1], vertex_buffer[i2].position[2]);
		glm::vec3 norm = glm::cross((p1 - p0), (p2 - p0));
		
		vertex_buffer[i0].normal[0] += norm.x;
		vertex_buffer[i0].normal[1] += norm.y;
		vertex_buffer[i0].normal[2] += norm.z;

		vertex_buffer[i1].normal[0] += norm.x;
		vertex_buffer[i1].normal[1] += norm.y;
		vertex_buffer[i1].normal[2] += norm.z;

		vertex_buffer[i2].normal[0] += norm.x;
		vertex_buffer[i2].normal[1] += norm.y;
		vertex_buffer[i2].normal[2] += norm.z;
	}

	for (unsigned int i=0; i < vertCnt; i++)
	{
		glm::vec3 norm(vertex_buffer[buf_vertex_count + i].normal[0], 
					vertex_buffer[buf_vertex_count + i].normal[1], 
					vertex_buffer[buf_vertex_count + i].normal[2]);
		norm = glm::normalize(norm);
		vertex_buffer[buf_vertex_count + i].normal[0] = norm.x;
		vertex_buffer[buf_vertex_count + i].normal[1] = norm.y;
		vertex_buffer[buf_vertex_count + i].normal[2] = norm.z;
	}

	return true;
}

void Buffer::load_buffers()
{
	if (loaded) return;
	glGenBuffers(1, &vertex_buffer_id);
	assert(vertex_buffer_id != 0);
	glGenBuffers(1, &color_buffer_id);
	assert(color_buffer_id != 0);
	glGenBuffers(1, &index_buffer_id);
	assert(index_buffer_id != 0);

	update_buffer_count();
	
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
    glBufferData(GL_ARRAY_BUFFER, buf_vertex_count * sizeof(VertexStruct), &vertex_buffer[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, color_buffer_id);
    glBufferData(GL_ARRAY_BUFFER, buf_vertex_count * sizeof(ColorStruct), &color_buffer[0], GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, buf_index_count * sizeof(IndexType), &index_buffer[0], GL_STATIC_DRAW);

	unbind();

	// clear memory after loading to GPU
	clear_buffers();
	loaded = true;
}

void Buffer::clear_buffers()
{
	vertex_buffer.clear();
	color_buffer.clear();
	index_buffer.clear();
}

void Buffer::draw(GLint position_id, GLint normal_id, GLint color_id)
{
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);

    glVertexAttribPointer(position_id, 3, GL_FLOAT, GL_FALSE,
        sizeof(VertexStruct), (void*) offsetof(VertexStruct, position));

    glVertexAttribPointer(normal_id, 3, GL_FLOAT, GL_FALSE,
        sizeof(VertexStruct), (void*) offsetof(VertexStruct, normal));

	glBindBuffer(GL_ARRAY_BUFFER, color_buffer_id);

    glVertexAttribPointer(color_id, 4, GL_UNSIGNED_BYTE, GL_TRUE,
        sizeof(ColorStruct), (void*) offsetof(ColorStruct, color));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_id);
	glDrawElements(type, buf_index_count, GL_UNSIGNED_INT, NULL);

	unbind();
}

void Buffer::change_color(uint32_t vertex_offset, uint32_t vertex_count, const RGBA& color)
{
	ColorStruct clr;
	clr.color[0] = color.r();
	clr.color[1] = color.g();
	clr.color[2] = color.b();
	clr.color[3] = color.a();
	
	std::vector<ColorStruct> colors;
	for (uint32_t i=0; i < vertex_count; i++)
		colors.push_back(clr);

	glBindBuffer(GL_ARRAY_BUFFER, color_buffer_id);
	glBufferSubData(GL_ARRAY_BUFFER, vertex_offset * sizeof(ColorStruct), 
		vertex_count * sizeof(ColorStruct), &colors[0]);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


