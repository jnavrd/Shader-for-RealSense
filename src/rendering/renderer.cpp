
#include "renderer.h"


renderer::renderer(int window_width, int window_height, const char *title) :
window_height_(window_height),
window_width_(window_width),
title_(title),
texture_loaded_(false),
shader_loaded_(false) {}

bool renderer::init() {
    InitWindow(window_width_, window_height_, title_);

    //check if shader is ready
    shader_ = LoadShader(NULL, "shaders/depth_variance.glsl");
    if (!IsShaderReady(shader_))
    {
        TraceLog(LOG_ERROR, "Custom shader failed to load");
        return false;
    }

    //check if uniforms are found
    texture_loc_ = GetShaderLocation(shader_, "texture0");
    min_range_loc_ = GetShaderLocation(shader_, "min_range");
    max_range_loc_ = GetShaderLocation(shader_, "max_range");
   /* if(texture_loc_ == -1 || min_range_loc_ == -1 || max_range_loc_ == -1)
    {
        TraceLog(LOG_ERROR, "uniform not found");
        return false;
    }*/

    shader_loaded_ = true;
    return true;
}

void renderer::update_texture(GrayscaleImg &img) {

    if(img.data.empty())
        return;

    Image image = {
      .data = img.data.data(),
      .width = img.width,
      .height = img.height,
      .mipmaps = 1,
      .format = PIXELFORMAT_UNCOMPRESSED_GRAYSCALE
    };

    if(!texture_loaded_)
    {
        texture_ = LoadTextureFromImage(image);
        texture_loaded_ = true;
    } else
    {
        UpdateTexture(texture_, image.data);
    }

}

void renderer::update_texture(DepthDataFloat depth_data) {
    if(depth_data.frame.empty())
        return;

    Image image = {
            .data = depth_data.frame.data(),
            .width = depth_data.width,
            .height = depth_data.height,
            .mipmaps = 1,
            .format = PIXELFORMAT_UNCOMPRESSED_R32
    };

    if(!texture_loaded_)
    {
        texture_ = LoadTextureFromImage(image);
        texture_loaded_ = true;
    } else
    {
        UpdateTexture(texture_, image.data);
    }
}

void renderer::set_shader_params(float min, float max) {

    SetShaderValue(shader_, min_range_loc_, &min, SHADER_UNIFORM_FLOAT);
    SetShaderValue(shader_, max_range_loc_, &max, SHADER_UNIFORM_FLOAT);
}

void renderer::render() {

    if(!shader_loaded_ || !texture_loaded_)
        return;

    BeginDrawing();
        ClearBackground(BLUE);
        BeginShaderMode(shader_);

            SetShaderValueTexture(shader_, texture_loc_, texture_);
            DrawTexturePro(
                    texture_,
                    Rectangle{ 0, 0, (float)texture_.width, (float)texture_.height },
                    Rectangle{ 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() },
                    Vector2{ 0, 0 },
                    0.0f,
                    WHITE
            );

        EndShaderMode();
    EndDrawing();
}

bool renderer::should_close() {
    return WindowShouldClose();
}

void renderer::close() {
    UnloadTexture(texture_);
    UnloadShader(shader_);
    CloseWindow();
}

renderer::~renderer() = default;