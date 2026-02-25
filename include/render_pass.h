//
// Created by juana on 24/2/2026.
//

#ifndef CUSTOMSHADERSRS_RENDER_PASS_H
#define CUSTOMSHADERSRS_RENDER_PASS_H

#include <string>
#include "raylib.h"

//strategy
class render_pass {
public:
    explicit render_pass(const std::string& path);
    virtual ~render_pass() = default;

    virtual bool init() = 0;
    virtual void render(Texture2D texture) = 0;

    virtual Shader load_shader(const std::string& path);
    virtual bool load_shader_uniforms() = 0;

    virtual void set_min_max_parameters(float min, float max);

    virtual void unload_shaders() = 0;

protected:

    std::string shader_path_;
    Shader display_shader_;

    int texture_loc_;

    int current_time_;
    int min_range_loc_;
    int max_range_loc_;

    bool shader_loaded_;
};

#endif //CUSTOMSHADERSRS_RENDER_PASS_H
