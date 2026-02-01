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

    void init();
    void update_texture(GrayscaleImg& img);
    void update_texture(DepthDataFloat depth_data);
    void set_shader_params(float depth_scale);
    void set_shader_params(float depth_scale, float min, float max);
    void render();
    void render_with_quad();
    static bool should_close() ;  // wrapper de WindowShouldClose
    void close();

private:
    int window_width_;
    int window_height_;
    const char *title_;
    Texture2D texture_;  // textura activa en GPU
    bool texture_loaded_;
    Shader shader_;
    bool shader_loaded_;
    int texture_loc_;
};
#endif //CUSTOMSHADERSRS_renderer_H