#pragma once

#include "glad/gl.h"
#include "glm.hpp"

/*
	TODO: add support for samplers and textures
*/

// All the data types we can use in a shader.
// These can be used for uniforms or VertexArray layouts
enum class ShaderDataType {
	F32, F64,
	Vec2, Vec3, Vec4,
	Mat2, Mat3, Mat4,
	Color,
	I8, I16, I32,
	U8, U16, U32,

	IVec2, IVec3, IVec4,

	Unknown=-1
};


constexpr ShaderDataType GetShaderDataType(GLenum dt) {
	switch (dt) {
		case GL_FLOAT: return ShaderDataType::F32;
		case GL_DOUBLE: return ShaderDataType::F64;

		case GL_FLOAT_VEC2: return ShaderDataType::Vec2;
		case GL_FLOAT_VEC3: return ShaderDataType::Vec3;
		case GL_FLOAT_VEC4: return ShaderDataType::Vec4;

		case GL_INT: return ShaderDataType::I32;
		case GL_UNSIGNED_INT: return ShaderDataType::U32;

		case GL_FLOAT_MAT2: return ShaderDataType::Mat2;
		case GL_FLOAT_MAT3: return ShaderDataType::Mat3;
		case GL_FLOAT_MAT4: return ShaderDataType::Mat4;

	}

	return ShaderDataType::Unknown;
}


// Returns the primitive type behind the ShaderDataType
constexpr GLenum GetGLPrimitiveType(ShaderDataType dt) {
	switch (dt) {
		case ShaderDataType::F32:	return GL_FLOAT;
		case ShaderDataType::F64:	return GL_DOUBLE;

		case ShaderDataType::Vec2:	return GL_FLOAT;
		case ShaderDataType::Vec3:	return GL_FLOAT;
		case ShaderDataType::Vec4:	return GL_FLOAT;

		case ShaderDataType::Mat2:	return GL_FLOAT;
		case ShaderDataType::Mat3:	return GL_FLOAT;
		case ShaderDataType::Mat4:	return GL_FLOAT;

		case ShaderDataType::Color:	return GL_FLOAT;

		case ShaderDataType::IVec2:	return GL_INT;
		case ShaderDataType::IVec3:	return GL_INT;
		case ShaderDataType::IVec4:	return GL_INT;

		case ShaderDataType::I8:	return GL_BYTE;
		case ShaderDataType::I16:	return GL_SHORT;
		case ShaderDataType::I32:	return GL_INT;

		case ShaderDataType::U8:	return GL_UNSIGNED_BYTE;
		case ShaderDataType::U16:	return GL_UNSIGNED_SHORT;
		case ShaderDataType::U32:	return GL_UNSIGNED_INT;
	}

	assert(false, "Unknown Data Type");
	return -1;
}

constexpr size_t GetGLPrimitiveSize(GLenum t) {
	switch (t) {
		case GL_FLOAT: return 4;
		case GL_DOUBLE: return 8;

		case GL_BYTE: return 1;
		case GL_SHORT: return 2;
		case GL_INT: return 4;
	
		case GL_UNSIGNED_BYTE: return 1;
		case GL_UNSIGNED_SHORT: return 2;
		case GL_UNSIGNED_INT: return 4;
	}
}


// Returns the primitive count behind the ShaderDataType
constexpr size_t GetGLPrimitiveCount(ShaderDataType dt) {
	switch (dt) {
	case ShaderDataType::F32:	return 1;
	case ShaderDataType::F64:	return 1;

	case ShaderDataType::Vec2:	return 2;
	case ShaderDataType::Vec3:	return 3;
	case ShaderDataType::Vec4:	return 4;

	case ShaderDataType::Mat2:	return 2 * 2;
	case ShaderDataType::Mat3:	return 3 * 3;
	case ShaderDataType::Mat4:	return 4 * 4;

	case ShaderDataType::Color:	return 3;

	case ShaderDataType::IVec2:	return 2;
	case ShaderDataType::IVec3:	return 3;
	case ShaderDataType::IVec4:	return 4;

	case ShaderDataType::I8:	return 1;
	case ShaderDataType::I16:	return 1;
	case ShaderDataType::I32:	return 1;

	case ShaderDataType::U8:	return 1;
	case ShaderDataType::U16:	return 1;
	case ShaderDataType::U32:	return 1;
	}
	assert(false, "Unknown Data Type");
	return -1;
}

template<ShaderDataType dt>
constexpr auto CreateGLType() {
	assert(false, "Unknown Data Type");
	return float();
}

#define SHADER_TYPE(x, y) template<> constexpr auto CreateGLType<x>() { return y(); }

SHADER_TYPE(ShaderDataType::F32, float);
SHADER_TYPE(ShaderDataType::F64, double);

SHADER_TYPE(ShaderDataType::I8, int8_t);
SHADER_TYPE(ShaderDataType::I16, int16_t);
SHADER_TYPE(ShaderDataType::I32, int32_t);

SHADER_TYPE(ShaderDataType::U8, uint8_t);
SHADER_TYPE(ShaderDataType::U16, uint16_t);
SHADER_TYPE(ShaderDataType::U32, uint32_t);

SHADER_TYPE(ShaderDataType::Vec2, glm::vec2);
SHADER_TYPE(ShaderDataType::Vec3, glm::vec3);
SHADER_TYPE(ShaderDataType::Vec4, glm::vec4);


SHADER_TYPE(ShaderDataType::Mat2, glm::mat2);
SHADER_TYPE(ShaderDataType::Mat3, glm::mat3);
SHADER_TYPE(ShaderDataType::Mat4, glm::mat4);

SHADER_TYPE(ShaderDataType::Color, glm::vec4);

SHADER_TYPE(ShaderDataType::IVec2, glm::ivec2);
SHADER_TYPE(ShaderDataType::IVec3, glm::ivec3);
SHADER_TYPE(ShaderDataType::IVec4, glm::ivec4);

#undef SHADER_TYPE


template<ShaderDataType dt>
constexpr auto CreatePrim() {
	assert(false, "Unknown Data Type");
	return float();
}

#define SHADER_PRIMITIVE(x, y) template<> constexpr auto CreatePrim<x>() { return y(); }

SHADER_PRIMITIVE(ShaderDataType::F32, float);
SHADER_PRIMITIVE(ShaderDataType::F64, double);

SHADER_PRIMITIVE(ShaderDataType::I8, int8_t);
SHADER_PRIMITIVE(ShaderDataType::I16, int16_t);
SHADER_PRIMITIVE(ShaderDataType::I32, int32_t);

SHADER_PRIMITIVE(ShaderDataType::U8, uint8_t);
SHADER_PRIMITIVE(ShaderDataType::U16, uint16_t);
SHADER_PRIMITIVE(ShaderDataType::U32, uint32_t);

SHADER_PRIMITIVE(ShaderDataType::Vec2, float);
SHADER_PRIMITIVE(ShaderDataType::Vec3, float);
SHADER_PRIMITIVE(ShaderDataType::Vec4, float);

SHADER_PRIMITIVE(ShaderDataType::Mat2, float);
SHADER_PRIMITIVE(ShaderDataType::Mat3, float);
SHADER_PRIMITIVE(ShaderDataType::Mat4, float);


SHADER_PRIMITIVE(ShaderDataType::Color, float);

SHADER_PRIMITIVE(ShaderDataType::IVec2, int32_t);
SHADER_PRIMITIVE(ShaderDataType::IVec3, int32_t);
SHADER_PRIMITIVE(ShaderDataType::IVec4, int32_t);

#undef SHADER_PRIMITIVE


// Reutrns the C type corresponding to a ShaderDataType
#define GetCType(x) decltype(CreateGLType<x>())

#define GetCPrimitiveType(x) decltype(CreatePrim<x>())

// Returns the size of a ShaderDataType
constexpr size_t getDataSize(ShaderDataType dt) {
	return GetGLPrimitiveSize(GetGLPrimitiveType(dt)) * GetGLPrimitiveCount(dt);
}
