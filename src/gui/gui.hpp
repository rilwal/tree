#pragma once

#include "imgui.h"

#include "renderer/shader.hpp"
#include "renderer/types.hpp"

struct GLFWwindow;

void init_gui(GLFWwindow* window);
void gui_new_frame();
void gui_end_frame();

void gui_uniform(const char* label, Uniform& uniform);
