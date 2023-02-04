#include <cstdio>
#include <iostream>
#include <algorithm> 
#include <format>

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"

#include "renderer/renderer.hpp"
#include "renderer/types.hpp"
#include "renderer/vertex_buffer.hpp"
#include "renderer/shader.hpp"
#include "renderer/camera.hpp"

#include "util.hpp"


int main() {
    AssetManager asset_manager;

    Renderer::initialize();
    
    Uniform x(ShaderDataType::Mat4);

    auto w = x.get<GetCType(ShaderDataType::Mat4)>();

    auto& shader = *asset_manager.LoadAsset<Shader>("assets/shaders/tree.glsl");


    while (true) {
        Renderer::begin_frame();

        
        shader.use();


        Renderer::end_frame();
    }

    Renderer::shutdown();

    return 0;
}