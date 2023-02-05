
#include "shader.hpp"
#include "glad/gl.h"

#include <vector>
#include <iostream>

#include "../util.hpp"

// Utility function to compile a GL shader from std::string
// TODO (High): Error handling here!
static uint32_t compile_gl_shader(GLenum type, const std::string src) {
	uint32_t shader = glCreateShader(type);
	const char* src_c = src.data();
	int32_t length = src.length();
	glShaderSource(shader, 1, &src_c, &length);

	glCompileShader(shader);

	return shader;
}


Shader::Shader() : IAsset("Shader") {

}

Shader::~Shader() {

}


// Skip until the next non-whitespace character
inline static void skip_whitespace(const char*& it) {
	while (std::isspace(*it)) it++;
}

// Skip until the next newline character
inline static void skip_to_newline(const char*& it) {
	while (*it != '\n') it++;
}


inline static bool check_token(const char*& it, const std::string& str) {
	if (strncmp(it, str.c_str(), str.length()) == 0) {
		return true;
	}
	return false;
}



inline static bool consume_token(const char*& it, const std::string& str) {
	if (check_token(it, str)) {
		it += str.length();
		return true;
	}
	return false;
}



void Shader::load_from_file(const char* filename) {
	std::vector<uint8_t> src = load_file(filename);
	load_from_string((const char*)src.data());
}



// Utility function to load a shaders source
// it should be a pointer to the first character in the string after a #type directive
uint32_t load_shader_src(GLenum type, const char* it) {
	// Find the next #directive
	std::vector<const char*> sources = {"#version 450\n"};
	std::vector<int32_t> lengths = {13};

	skip_whitespace(it);
	sources.push_back(it);

	while (*it) {
		if (check_token(it, "#type")) {
			lengths.push_back(it - sources[sources.size() - 1]);

			printf("End of shader source\n");
			break;
		}
		
		if (check_token(it, "#include")) {
			// handle the include here
			lengths.push_back(it - sources[sources.size() - 1]);
			skip_to_newline(it);
			skip_whitespace(it);

			sources.push_back(it);
		}

		else {
			skip_to_newline(it);
			skip_whitespace(it);
		}
	}

	if (!*it) {
		lengths.push_back(it - sources[sources.size() - 1]);

		printf("End of shader source\n");
	}

	uint32_t shader = glCreateShader(type);

	glShaderSource(shader, sources.size(), sources.data(), lengths.data());

	glCompileShader(shader);

	GLint _compiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &_compiled);

	if (!_compiled) {
		std::string info_log;

		int32_t log_length = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);
		info_log.resize(log_length);
		glGetShaderInfoLog(shader, log_length, &log_length, &info_log[0]);
	
		std::cerr << "Failed to compile shader:\n" << info_log;

		glDeleteShader(shader);

		return -1;
	}


	return shader;
}



void Shader::load_from_string(const char* src) {
	// Use old fashioned C methods:
	// In my experience it's faster and it's not so hard to write
	// This might be premature optimization, but I feel like writing it this way today 😂

	// TODO: Make tokens non-case-sensitive
	// TODO: Deal with uniform related directives
	linked = false;

	fragment_shader = 0;


	gl_id = glCreateProgram();

	const char* it = src;
	skip_whitespace(it);

	// Go to the end of the file
	while (*it) {
		// A directive we care about
		if (*it == '#') {
			if (consume_token(it, "#type")) {
				skip_whitespace(it);

				if (consume_token(it, "fragment") || consume_token(it, "pixel")) {
					uint32_t shader = load_shader_src(GL_FRAGMENT_SHADER, it);
					if (shader != -1) {
						fragment_shader = shader;
						glAttachShader(gl_id, shader);
					}
				}
				
				else if (consume_token(it, "vertex")) {
					uint32_t shader = load_shader_src(GL_VERTEX_SHADER, it);
					if (shader != -1) {
						vertex_shader = shader;
						glAttachShader(gl_id, shader);
					}
				}
				
				else if (consume_token(it, "compute")) {
					uint32_t shader = load_shader_src(GL_COMPUTE_SHADER, it);
					if (shader != -1) {
						compute_shader = shader;
						glAttachShader(gl_id, shader);
					}
				}

				else if (consume_token(it, "geometry")) {
					uint32_t shader = load_shader_src(GL_GEOMETRY_SHADER, it);
					if (shader != -1) {
						geometry_shader = shader;
						glAttachShader(gl_id, shader);
					}
				}

				else if (consume_token(it, "tess_con")) {
					uint32_t shader = load_shader_src(GL_TESS_CONTROL_SHADER, it);
					if (shader != -1) {
						tess_con_shader = shader;
						glAttachShader(gl_id, shader);
					}
				}

				else if (consume_token(it, "tess_eval")) {
					uint32_t shader = load_shader_src(GL_TESS_EVALUATION_SHADER, it);
					if (shader != -1) {
						tess_eval_shader = shader;
						glAttachShader(gl_id, shader);
					}
				}

				else {
					skip_to_newline(it);
					skip_whitespace(it);
				}

			}
			else {
				skip_to_newline(it);
				skip_whitespace(it);
			}
		}
		else {
			skip_to_newline(it);
			skip_whitespace(it);
		}
	}

	// Now link the program
	glLinkProgram(gl_id);

	if (fragment_shader)	{ glDetachShader(gl_id, fragment_shader);	glDeleteShader(fragment_shader);  }
	if (vertex_shader)		{ glDetachShader(gl_id, vertex_shader);		glDeleteShader(vertex_shader);    }
	if (compute_shader)		{ glDetachShader(gl_id, compute_shader);	glDeleteShader(compute_shader);   }
	if (geometry_shader)	{ glDetachShader(gl_id, geometry_shader);	glDeleteShader(geometry_shader);  }
	if (tess_con_shader)	{ glDetachShader(gl_id, tess_con_shader);	glDeleteShader(tess_con_shader);  }
	if (tess_eval_shader)	{ glDetachShader(gl_id, tess_eval_shader);	glDeleteShader(tess_eval_shader); }

	GLint _linked;
	glGetProgramiv(gl_id, GL_LINK_STATUS, &_linked);

	if (_linked) linked = true;

	if (!linked) {
		int32_t log_length = 0;
		glGetProgramiv(gl_id, GL_INFO_LOG_LENGTH, &log_length);
		info_log.resize(log_length);
		glGetProgramInfoLog(gl_id, log_length, &log_length, info_log.data());// &info_log[0]);

		printf("%s\n", info_log.data());
		return;
	}

	// finally, introspect the shader!
	introspect();

}

void Shader::introspect() {
	// TODO: SUpport all types
	// TODO: Persist across reloads
	int num_active_uniforms = 0;
	glGetProgramiv(gl_id, GL_ACTIVE_UNIFORMS, &num_active_uniforms);

	for (int i = 0; i < num_active_uniforms; i++) {
		char uniform_name[1024];
		int length;
		uint32_t uniform_type;
		int uniform_size;

		glGetActiveUniform(gl_id, i, 1024, &length, &uniform_size, &uniform_type, uniform_name);


		ShaderDataType t = GetShaderDataType(uniform_type);
		auto scr = strncmp(uniform_name, "c_", 2);
		if (t == ShaderDataType::Vec3 && scr == 0) t = ShaderDataType::Color;

		Uniform u(t);

		u.location = glGetUniformLocation(gl_id, uniform_name);
		u.name = uniform_name;

		switch (t) {
		case ShaderDataType::F32:			
			glGetUniformfv(gl_id, u.location, &u.get<float>());
			break;
		case ShaderDataType::Vec3:
		case ShaderDataType::Color:

			glGetUniformfv(gl_id, u.location, &u.get<glm::vec3>()[0]);
			break;
		}
	
		uniforms[u.name] = u;
	}
}

void Shader::use() {

	glUseProgram(gl_id);


	// This should probably not be done this way, but it's how we're doing it for now 😂
	for (auto& [name, uniform] : uniforms) {
		switch (uniform.type) {
			// Let's do another macro thing here to save a lot of chars
			// Dear god, what have I created????
#define UNIFORM_TYPE(shader_type, gl_func) case ShaderDataType::shader_type: gl_func(uniform.location, 1, (GetCPrimitiveType(ShaderDataType::shader_type)*)&uniform.get<GetCType(ShaderDataType::shader_type)>()); break;
#define MAT_UNIFORM_TYPE(shader_type, gl_func) case ShaderDataType::shader_type: gl_func(uniform.location, 1, false, (GetCPrimitiveType(ShaderDataType::shader_type)*)&uniform.get<GetCType(ShaderDataType::shader_type)>()); break;

			UNIFORM_TYPE(F32, glUniform1fv);
			UNIFORM_TYPE(F64, glUniform1dv);
			UNIFORM_TYPE(Vec2, glUniform2fv);
			UNIFORM_TYPE(Vec3, glUniform3fv);
			UNIFORM_TYPE(Vec4, glUniform4fv);

			MAT_UNIFORM_TYPE(Mat2, glUniformMatrix2fv);
			MAT_UNIFORM_TYPE(Mat3, glUniformMatrix3fv);
			MAT_UNIFORM_TYPE(Mat4, glUniformMatrix4fv);

			UNIFORM_TYPE(Color, glUniform3fv);

			UNIFORM_TYPE(I32, glUniform1iv);
			UNIFORM_TYPE(U32, glUniform1uiv);
#undef UNIFORM_TYPE
#undef MAT_UNIFORM_TYPE


		default:
			assert("UNKNOWN DATA TYPE");

		}
	}

}


void Shader::unload() {
	glDeleteProgram(gl_id);
	gl_id = 0;
}