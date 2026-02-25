//
// Created by juana on 24/2/2026.
//

#ifndef CUSTOMSHADERSRS_FEEDBACK_PASS_H
#define CUSTOMSHADERSRS_FEEDBACK_PASS_H

#include "render_pass.h"

class feedback_pass : public render_pass{
public:
    feedback_pass(const std::string& display_path, const std::string& sim_path, int w_width, int w_height);

    bool init() override;

    void render(Texture2D texture) override;

    bool load_shader_uniforms() override;
    bool load_sim_shader_uniforms();

    void unload_shaders() override;

private:
    int window_width_;
    int window_height_;

    RenderTexture2D prev_texture_;
    RenderTexture2D texture_buffers[2];
    int current_sim_buffer_ = 0;

    std::string simulation_shader_path_;
    Shader simulation_shader_; // propagation shader

    //uniforms location
    int ripple_state_texture_loc_;

    int depth_texture_loc_;
    int prev_depth_texture_loc_;
    int prev_state_loc_;

    int sim_time_loc_;

};


#endif //CUSTOMSHADERSRS_FEEDBACK_PASS_H
