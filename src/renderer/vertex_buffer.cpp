
#include "vertex_buffer.hpp"

VertexBuffer::VertexBuffer()
{
	glCreateVertexArrays(1, &vao_id);
	glCreateBuffers(1, &vbo_id);
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &vbo_id);
	glDeleteVertexArrays(1, &vao_id);
}

std::vector<VertexAttribute>::iterator VertexBufferLayout::begin() {
	return vertex_attributes.begin();
}


std::vector<VertexAttribute>::iterator VertexBufferLayout::end() {
	return vertex_attributes.end();
}

