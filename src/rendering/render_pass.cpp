//
// Created by juana on 24/2/2026.
//

#include "render_pass.h"

render_pass::render_pass(const std::string &path) : shader_path_(path), shader_loaded_(false),
                                                    texture_loc_(-1),
                                                    current_time_(0),
                                                    min_range_loc_(0),
                                                    max_range_loc_(0){}

Shader render_pass::load_shader(const std::string &path) {
    return LoadShader(NULL, path.c_str());
}

void render_pass::set_min_max_parameters(float min, float max) {
    SetShaderValue(display_shader_, min_range_loc_, &min, SHADER_UNIFORM_FLOAT);
    SetShaderValue(display_shader_, max_range_loc_, &max, SHADER_UNIFORM_FLOAT);
}