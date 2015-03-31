#include "buffer.h"
#include <glm/glm.hpp>

using namespace renderlib;

#define MAX_VERTEX_COUNT 150000
#define MAX_INDEX_COUNT 600000

Buffer::Buffer():
	vertex_buffer_id(0), index_buffer_id(0),
	buf_vertex_count(0), buf_index_count(0),
	transparent(false), type(GL_TRIANGLES),
	bounded(false), loaded(false)
{
}

Buffer::~Buffer()
{
	clear_buffers();
}

void Buffer::bind()
{
	if (bounded) return;
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_id);
	bounded = true;
}

void Buffer::unbind()
{
	if (!bounded) return;
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	bounded = false;
}

void Buffer::update_buffer_count()
{
	buf_vertex_count = vertex_buffer.size();
	buf_index_count = index_buffer.size();
}

bool Buffer::add_ctm_data(CTMDecoder& ctm, const glm::mat4& matrix, const RGBA& color, Box& bbox)
{
	auto vertCnt = ctm.get_vertex_count();
	auto triCnt = ctm.get_tri_count();
	auto indexCnt = triCnt * 3;

	update_buffer_count();

	if ((buf_vertex_count + vertCnt) > MAX_VERTEX_COUNT || (buf_index_count + indexCnt) > MAX_INDEX_COUNT)
		return false;

	//vertex_buffer.reserve(buf_vertex_count + vertCnt);
	//index_buffer.reserve(buf_index_count + indexCnt);

	const float* ctm_vertices = ctm.get_vertices();
	const unsigned int* ctm_indices = ctm.get_indices();

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

		vertex.color[0] = color.r();
		vertex.color[1] = color.g();
		vertex.color[2] = color.b();
		vertex.color[3] = color.a();

		vertex_buffer.push_back(vertex);
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
		glm::vec3 va = p1 - p0;
		glm::vec3 vb = p2 - p0;
		glm::vec3 norm = glm::normalize(glm::cross(va, vb));
		
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
	glGenBuffers(1, &index_buffer_id);
	assert(index_buffer_id != 0);

	update_buffer_count();
	
	bind();
    glBufferData(GL_ARRAY_BUFFER, buf_vertex_count * sizeof(VertexStruct), &vertex_buffer[0], GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, buf_index_count * sizeof(IndexType), &index_buffer[0], GL_STATIC_DRAW);
	unbind();

	// clear memory after loading to GPU
	clear_buffers();
	loaded = true;
}

void Buffer::clear_buffers()
{
	vertex_buffer.clear();
	index_buffer.clear();
}

void Buffer::draw(GLint position_id, GLint normal_id, GLint color_id)
{
	bind();

    glVertexAttribPointer(position_id, 3, GL_FLOAT, GL_FALSE,
        sizeof(VertexStruct), (void*) offsetof(VertexStruct, position));

    glVertexAttribPointer(normal_id, 3, GL_FLOAT, GL_FALSE,
        sizeof(VertexStruct), (void*) offsetof(VertexStruct, normal));

    glVertexAttribPointer(color_id, 4, GL_UNSIGNED_BYTE, GL_TRUE,
        sizeof(VertexStruct), (void*) offsetof(VertexStruct, color));

	glDrawElements(type, buf_index_count, GL_UNSIGNED_INT, NULL);

	unbind();
}


