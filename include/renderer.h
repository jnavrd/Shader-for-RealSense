#ifndef CUSTOMSHADERSRS_RENDERER_H
#define CUSTOMSHADERSRS_RENDERER_H

#include <vector>
#include <iostream>
#include "raylib.h"
#include "rlgl.h"
#include "frame_types.h"

class renderer {
public:
    renderer(int window_width, int window_height, const char *title);
    ~renderer();

    bool init();

    [[maybe_unused]] void update_texture(GrayscaleImg& img);
    void update_texture(DepthDataFloat depth_data);

    void set_shader_params(float min, float max);

    void render();
    void render(DepthDataFloat depth_data); //for ping-pong shaders

    static bool should_close() ;  // wrapper de WindowShouldClose
    void close();

private:
    //window
    int window_width_;
    int window_height_;
    const char *title_;

    Texture2D texture_;  // textura activa en GPU
    RenderTexture2D prev_texture_;
    RenderTexture2D texture_buffers[2];
    int current_sim_buffer_ = 0;

    Shader change_detection_shader_;
    Shader simulation_shader_;
    Shader shader_; //display shader

    //uniforms location
    int texture_loc_;
    int ripple_state_texture_loc_;

    int sim_depth_texture_loc_;
    int prev_depth_texture_loc_;
    int prev_state_loc_;
    int sim_time_loc_;

    int current_time_;
    int min_range_loc_;
    int max_range_loc_;

    //flags
    bool texture_loaded_;
    bool shader_loaded_;

    //TODO SACAR
    bool loadDisplayShaderUniforms();
    bool loadSimulationShaderUniforms();
    bool loadChangeShaderUniforms();
};

#endif //CUSTOMSHADERSRS_renderer_H