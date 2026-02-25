//
// Created by juana on 24/2/2026.
//

#include "feedback_pass.h"
feedback_pass::feedback_pass(const std::string& path, const std::string& sim_path, int w_width, int w_height) : render_pass(path),
simulation_shader_path_(sim_path),
window_width_(w_width),
window_height_(w_height){}

bool feedback_pass::init() {
    display_shader_ = load_shader(shader_path_);
    simulation_shader_ = load_shader(simulation_shader_path_);

    if (!IsShaderReady(display_shader_)) {
        TraceLog(LOG_ERROR, "Custom display shader failed to load");
        return false;
    }
    if(!IsShaderReady(simulation_shader_)) {
        TraceLog(LOG_ERROR, "Simulation shader failed to load");
        return false;
    }

    //load textures
    prev_texture_ = LoadRenderTexture(window_width_,window_height_);

    texture_buffers[0] = LoadRenderTexture(window_width_, window_height_);
    texture_buffers[1] = LoadRenderTexture(window_width_, window_height_);
    for (const auto & texture_buffer : texture_buffers)
    {
        BeginTextureMode(texture_buffer);
        ClearBackground(BLACK);
        EndTextureMode();
    }


    if (!load_shader_uniforms()) {
        TraceLog(LOG_ERROR, "Display shader uniforms missing");
        return false;
    }
    if (!load_sim_shader_uniforms()) {
        TraceLog(LOG_ERROR, "Simulation shader uniforms missing");
        return false;
    }

    shader_loaded_ = true;
    return true;
}


bool feedback_pass::load_shader_uniforms() {
    texture_loc_ = GetShaderLocation(display_shader_, "depthData");
    ripple_state_texture_loc_ = GetShaderLocation(display_shader_, "rippleData");
    //current_time_ = GetShaderLocation(display_shader_, "current_time");
    min_range_loc_ = GetShaderLocation(display_shader_, "min_range");
    max_range_loc_ = GetShaderLocation(display_shader_, "max_range");

    return texture_loc_ != -1
    && ripple_state_texture_loc_ != -1
    && min_range_loc_ != -1
    && max_range_loc_ != -1;
}

bool feedback_pass::load_sim_shader_uniforms() {
    prev_state_loc_ = GetShaderLocation(simulation_shader_, "prevState");
    depth_texture_loc_ = GetShaderLocation(simulation_shader_, "depth");
    prev_depth_texture_loc_ = GetShaderLocation(simulation_shader_, "prevDepth");
    //sim_time_loc_ = GetShaderLocation(simulation_shader_, "current_time");

    return prev_state_loc_ != -1
    && depth_texture_loc_ != -1
    && prev_depth_texture_loc_ != -1
    /*&& sim_time_loc_ != -1*/;
}

void feedback_pass::render(Texture2D texture) {
    if(!shader_loaded_)
        return;

    int read_buffer = current_sim_buffer_;
    int write_buffer = 1 - current_sim_buffer_;

    // Simulation
    BeginTextureMode(texture_buffers[write_buffer]);
    BeginShaderMode(simulation_shader_);
    float time = (float)GetTime();

    Texture2D read = texture_buffers[read_buffer].texture;
    SetShaderValue(simulation_shader_, sim_time_loc_, &time, SHADER_UNIFORM_FLOAT);
    SetShaderValueTexture(simulation_shader_, prev_state_loc_, read);
    SetShaderValueTexture(simulation_shader_, depth_texture_loc_, texture);
    SetShaderValueTexture(simulation_shader_, prev_depth_texture_loc_, prev_texture_.texture);
    DrawTexturePro(
            read,
            Rectangle{0, 0, (float)read.width, -(float)read.height},
            Rectangle{0, 0, (float)read.width, (float)read.height},
            Vector2{0, 0},
            0.0f,
            WHITE
    );

    EndShaderMode();
    EndTextureMode();


    BeginDrawing();
    ClearBackground(BLACK);
    BeginShaderMode(display_shader_);

    Texture2D current = texture_buffers[write_buffer].texture;
    //ripple state
    SetShaderValueTexture(display_shader_, texture_loc_, texture);
    SetShaderValueTexture(display_shader_, ripple_state_texture_loc_, current);
    DrawTexturePro(
            texture,
            Rectangle{ 0, 0, (float)texture.width, (float)texture.height },
            Rectangle{ 0, 0, (float)window_width_, (float)window_height_ },
            Vector2{ 0, 0 },
            0.0f,
            WHITE
    );

    EndShaderMode();
    EndDrawing();

    current_sim_buffer_ = write_buffer;

    BeginTextureMode(prev_texture_);
    DrawTexturePro(
            texture,
            Rectangle{ 0, 0, (float)texture.width, -(float)texture.height },
            Rectangle{ 0, 0, (float)window_width_, (float)window_height_ },
            Vector2{ 0, 0 },
            0.0f,
            WHITE
    );
    EndTextureMode();

}

void feedback_pass::unload_shaders() {
    UnloadShader(display_shader_);
    UnloadShader(simulation_shader_);
}


