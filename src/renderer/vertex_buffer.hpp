#pragma once

#include <cinttypes>
#include <cassert>
#include <string>
#include <vector>

#include "types.hpp"

struct VertexAttribute {
	std::string name;
	ShaderDataType data_type;
	size_t offset = 0;
};


struct VertexBufferLayout {
	std::vector<VertexAttribute> vertex_attributes;

	VertexBufferLayout() {

	}

	VertexBufferLayout(std::initializer_list<VertexAttribute> il)
		: vertex_attributes(il) { };

	inline void append(const VertexAttribute& va) { vertex_attributes.push_back(va); };

	std::vector<VertexAttribute>::iterator begin();
	std::vector<VertexAttribute>::iterator end();

};

class VertexBuffer {
public:
	VertexBuffer();
	~VertexBuffer();

	void bind() {
		glBindVertexArray(vao_id);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
	}

	void set_layout(VertexBufferLayout layout) {
		bind();

		size_t size = 0;

		// First count the size of our layout
		for (const auto& attribute : layout) {
			size += getDataSize(attribute.data_type);
		}

		// Then loop over and set everything up
		size_t index = 0;
		size_t offset = 0;

		for (auto& attribute : layout) {

			const ShaderDataType& dt = attribute.data_type;

			//std::cout << std::format("glVertexArrayAttribFormat(id={}, index={}, GetSize(dt)={}, GetGLType(dt)={}, false, offset={});", m_id, index, GetSize(dt), GetGLType(dt), offset);

			glEnableVertexAttribArray(index);
			//glVertexAttribPointer(index, GetComponentCount(dt), GetGLType(dt), false, size, (const void*)offset);
			glVertexArrayAttribFormat(vao_id, index, GetGLPrimitiveCount(dt), GetGLPrimitiveType(dt), false, offset);
			glVertexArrayAttribBinding(vao_id, index, 0);
			attribute.offset = offset;

			offset += getDataSize(dt);
			index++;
		}

		glVertexArrayVertexBuffer(vao_id, 0, vbo_id, 0, size);

		m_stride = size;
	}

	void set_data(void* data, size_t len) {
		glNamedBufferData(vbo_id, len, data, GL_STATIC_DRAW);
	}

private:
	uint32_t vao_id;
	uint32_t vbo_id;

	uint32_t m_stride = 0;
};
