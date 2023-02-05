#pragma once

/*
	Shader handling for another-engine.

	Shader asset files will contain GLSL code with some markup for preprocessing.
	One Shader asset file defines a GLSL Program.

	All markup lines begin with #

	TODO: #include "filename" will include files in a way analogous to the C preprocessor
	#type {vertex / fragment / geometry / compute / tess_con / tess_eval} defines a new shader of the given type
	TODO: #range(min, max, step) before a uniform variable tells us the valid range for that uniform to display in any user interface

	TODO: consider this more
*/

#include <string>
#include <cstdint>
#include <vector>
#include <map>

#include "types.hpp"
#include "asset_manager.hpp"


struct Uniform {
	Uniform(): type(ShaderDataType::F32) { _data = {}; };
	Uniform(ShaderDataType dt) : type(dt) { _data = {}; };
	Uniform(const Uniform& u) : name(u.name), type(u.type), _data(u._data) {};

	std::string name;
	ShaderDataType type;
	uint32_t location = 0;

	template <typename t>
	auto& get() {
		assert(false);
	}

	template <typename t>
	void set(t val) {
		assert(false);
	}

	//take c type and data type and make getter
#define UniformGetter(ct, dt, m) template<> auto& get<ct>() { return _data.m; } 
	UniformGetter(float,  F32, f);
	UniformGetter(double, F64, d);
	UniformGetter(glm::vec2, Vec2, v2);
	UniformGetter(glm::vec3, Vec3, v3);
	UniformGetter(glm::vec4, Vec4, v4);
	UniformGetter(glm::ivec2, IVec2, iv2);
	UniformGetter(glm::ivec3, IVec3, iv3);
	UniformGetter(glm::ivec4, IVec4, iv4);
	UniformGetter(uint8_t, U8, u8);
	UniformGetter(uint16_t, U16, u16);
	UniformGetter(uint32_t, U32, u32);
	UniformGetter(int8_t, I8, i8);
	UniformGetter(int16_t, I16, i16);
	UniformGetter(int32_t, I32, i32);
	UniformGetter(glm::mat2, Mat2, m2);
	UniformGetter(glm::mat3, Mat3, m3);
	UniformGetter(glm::mat4, Mat4, m4);
#undef UniformGetter

#define UniformSetter(ct, dt, m) template<> void set<ct>(ct val) { assert(type == ShaderDataType::dt); _data.m = val; }
	UniformSetter(float, F32, f);
	UniformSetter(double, F64, d);
	UniformSetter(glm::vec2, Vec2, v2);
	UniformSetter(glm::vec3, Vec3, v3);
	UniformSetter(glm::vec4, Vec4, v4);
	UniformSetter(glm::ivec2, IVec2, iv2);
	UniformSetter(glm::ivec3, IVec3, iv3);
	UniformSetter(glm::ivec4, IVec4, iv4);
	UniformSetter(uint8_t, U8, u8);
	UniformSetter(uint16_t, U16, u16);
	UniformSetter(uint32_t, U32, u32);
	UniformSetter(int8_t, I8, i8);
	UniformSetter(int16_t, I16, i16);
	UniformSetter(int32_t, I32, i32);
	UniformSetter(glm::mat2, Mat2, m2);
	UniformSetter(glm::mat3, Mat3, m3);
	UniformSetter(glm::mat4, Mat4, m4);
#undef UniformSetter

private:
	union {
		float f;
		double d;
		glm::vec2 v2;
		glm::vec3 v3;
		glm::vec4 v4;

		glm::ivec2 iv2;
		glm::ivec3 iv3;
		glm::ivec4 iv4;

		uint8_t u8;
		uint16_t u16;
		uint32_t u32;
		
		int8_t i8;
		int16_t i16;
		int32_t i32;
		
		glm::mat2 m2;
		glm::mat3 m3;
		glm::mat4 m4;
	} _data;
};


class Shader : public IAsset {
public:
	Shader();
	~Shader();

	void load_from_file(const char* filename) override;
	void load_from_string(const char* src);

	void unload() override;

	void introspect();

	void use();

	inline uint32_t get_id() { return gl_id; }

	std::map<std::string, Uniform> uniforms;
private:
	bool linked = false;

	std::vector<char> info_log;

	uint32_t fragment_shader	= 0;
	uint32_t vertex_shader		= 0;
	uint32_t compute_shader		= 0;
	uint32_t geometry_shader	= 0;
	uint32_t tess_con_shader	= 0;
	uint32_t tess_eval_shader	= 0;

	uint32_t gl_id;

};
