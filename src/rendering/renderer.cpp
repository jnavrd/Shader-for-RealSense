
#include "renderer.h"


renderer::renderer(int window_width, int window_height, const char *title) :
window_height_(window_height), window_width_(window_width), title_(title), texture_loaded_(false), shader_loaded_(false) {

}

void renderer::init() {
    InitWindow(window_width_, window_height_, title_);

    //shader_ = LoadShader(NULL, "shaders/depth_grayscale.glsl");
    //test
    shader_ = LoadShader("shaders/fullscreen.fs", "shaders/depth_grayscale.glsl");

    if (!IsShaderReady(shader_))
    {
        TraceLog(LOG_ERROR, "CUSTOM SHADER NOT READY");
    }
    else
    {
        TraceLog(LOG_INFO, "CUSTOM SHADER READY");
    }
    texture_loc_ = GetShaderLocation(shader_, "texture0");
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

void renderer::set_shader_params(float depth_scale) {

    int depthScaleLoc = GetShaderLocation(shader_, "depth_scale");

    SetShaderValue(shader_, depthScaleLoc, &depth_scale, SHADER_UNIFORM_FLOAT);
}

void renderer::set_shader_params(float depth_scale, float min, float max) {

}

void renderer::render() {
    BeginDrawing();
        ClearBackground(BLACK);
        BeginShaderMode(shader_);
            DrawTextureEx(texture_, Vector2{ 0.0f, 0.0f }, 0.0f, 2.0f, WHITE); //TODO dynamic scaling
        EndShaderMode();
    EndDrawing();
}

void renderer::render_with_quad() {

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
        //Draw quad
       /* rlBegin(RL_QUADS);

            // Top-left
            rlTexCoord2f(0.0f, 0.0f);
            rlVertex2f(0.0f, 0.0f);

            // Top-right
            rlTexCoord2f(1.0f, 0.0f);
            rlVertex2f((float)window_width_, 0.0f);

            // Bottom-right
            rlTexCoord2f(1.0f, 1.0f);
            rlVertex2f((float)window_width_, (float)window_height_);

            // Bottom-left
            rlTexCoord2f(0.0f, 1.0f);
            rlVertex2f(0.0f, (float)window_height_);

        rlEnd();*/

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