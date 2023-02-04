
#include "renderer.hpp"

#include "glad/gl.h"
#include "GLFW/glfw3.h"

struct {
	GLFWwindow* window;
} s_Data;


// Initialize OpenGL and shit
void Renderer::initialize() {
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	s_Data.window = glfwCreateWindow(1920, 1080, "Tree", nullptr, nullptr);

	glfwMakeContextCurrent(s_Data.window);
	gladLoadGL(glfwGetProcAddress);
}

void Renderer::shutdown() {
	glfwDestroyWindow(s_Data.window);
	
	glfwTerminate();
}


void Renderer::begin_frame() {
	glfwPollEvents();

	int32_t display_w, display_h;

	glfwGetFramebufferSize(s_Data.window, &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::end_frame() {
	glfwSwapBuffers(s_Data.window);
}

GLFWwindow* Renderer::get_window() {
	return s_Data.window;
}