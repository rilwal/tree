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

#include "gui/gui.hpp"

int main() {
    AssetManager asset_manager;

    Renderer::initialize();
    
    Uniform x(ShaderDataType::Mat4);

    auto w = x.get<GetCType(ShaderDataType::Mat4)>();

    auto& shader = *asset_manager.LoadAsset<Shader>("assets/shaders/tree.glsl");


    std::vector<glm::vec2> geometry = {
        {-1, 1},
        {1, -1},
        {-1, -1},
        {-1, 1},
        {1, 1},
        {1, -1}
    };


    VertexBuffer vb;
    VertexBufferLayout vbl = {
        {"pos", ShaderDataType::Vec2}
    };

    vb.set_layout(vbl);
    vb.set_data(geometry.data(), geometry.size() * sizeof(glm::vec2));

    init_gui(Renderer::get_window());

    while (true) {
        Renderer::begin_frame();
       
        gui_new_frame();


        bool tree_settings_open = true;


        if (ImGui::Begin("Tree settings", &tree_settings_open, 0)) {
            if (ImGui::Button("Reload shader")) {
                shader.load_from_file("assets/shaders/tree.glsl");
                shader.introspect();
            }

            for (auto& [name, uniform] : shader.uniforms) {
                gui_uniform(name.c_str(), uniform);
            }

            ImGui::End();
        }


        shader.use();
        vb.bind();

        glDrawArrays(GL_TRIANGLES, 0, 6);

        gui_end_frame();
        Renderer::end_frame();
    }

    Renderer::shutdown();

    return 0;
}