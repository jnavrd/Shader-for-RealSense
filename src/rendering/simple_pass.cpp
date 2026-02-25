//
// Created by juana on 24/2/2026.
//

#include "simple_pass.h"

simple_pass::simple_pass(const std::string& path) : render_pass(path){
}

bool simple_pass::init() {
    display_shader_ = load_shader(shader_path_);
    if(!IsShaderReady(display_shader_))
    {
        TraceLog(LOG_ERROR, "Custom display shader failed to load");
        return false;
    }
    if(!load_shader_uniforms())
    {
        TraceLog(LOG_ERROR, "Display shader uniforms missing");
        return false;
    }

    shader_loaded_ = true;

    return true;
}

void simple_pass::render(Texture2D texture) {
    if(!shader_loaded_)
        return;

    BeginDrawing();
    ClearBackground(BLUE);
    BeginShaderMode(display_shader_);

    SetShaderValueTexture(display_shader_, texture_loc_, texture);

    DrawTexturePro(
            texture,
            Rectangle{ 0, 0, (float)texture.width, (float)texture.height },
            Rectangle{ 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() },
            Vector2{ 0, 0 },
            0.0f,
            WHITE
    );

    EndShaderMode();
    EndDrawing();
}


bool simple_pass::load_shader_uniforms() {

    min_range_loc_ = GetShaderLocation(display_shader_, "min_range");
    max_range_loc_ = GetShaderLocation(display_shader_, "max_range");

    return min_range_loc_ != -1 && max_range_loc_ != -1;
}


void simple_pass::unload_shaders() {
    UnloadShader(display_shader_);
}