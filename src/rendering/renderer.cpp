
#include "renderer.h"


renderer::renderer(int window_width, int window_height, const char *title) :
window_height_(window_height),
window_width_(window_width),
title_(title),
texture_loaded_(false),
shader_loaded_(false) {}

bool renderer::init() {
    InitWindow(window_width_, window_height_, title_);

    //check if shaders are ready
    shader_ = LoadShader(NULL, "shaders/ripple/ripple_effect.glsl");
    simulation_shader_ = LoadShader(NULL, "shaders/ripple/simulation_shader.glsl");


    if (!IsShaderReady(shader_)) {
        TraceLog(LOG_ERROR, "Custom display shader failed to load");
        return false;
    }
    if(!IsShaderReady(simulation_shader_)) {
        TraceLog(LOG_ERROR, "Simulation shader failed to load");
        return false;
    }

    //load textures
    prev_texture_ = LoadRenderTexture(window_width_, window_height_);

    texture_buffers[0] = LoadRenderTexture(window_width_, window_height_);
    texture_buffers[1] = LoadRenderTexture(window_width_, window_height_);
    for (const auto & texture_buffer : texture_buffers)
    {
        BeginTextureMode(texture_buffer);
        ClearBackground(BLACK);
        EndTextureMode();
    }

    if (!loadDisplayShaderUniforms()) {
        TraceLog(LOG_ERROR, "Display shader uniforms missing");
        return false;
    }
    if (!loadSimulationShaderUniforms()) {
        TraceLog(LOG_ERROR, "Simulation shader uniforms missing");
        return false;
    }

    shader_loaded_ = true;
    return true;
}

bool renderer::loadDisplayShaderUniforms()
{
    bool ret = false;

    texture_loc_ = GetShaderLocation(shader_, "depthData");
    ripple_state_texture_loc_ = GetShaderLocation(shader_, "rippleData");
    //current_time_ = GetShaderLocation(shader_, "current_time");
    min_range_loc_ = GetShaderLocation(shader_, "min_range");
    max_range_loc_ = GetShaderLocation(shader_, "max_range");

    return texture_loc_ != -1 && ripple_state_texture_loc_ != -1;
}

bool renderer::loadSimulationShaderUniforms()
{

    prev_state_loc_ = GetShaderLocation(simulation_shader_, "prevState");
    sim_depth_texture_loc_ = GetShaderLocation(simulation_shader_, "depth");
    prev_depth_texture_loc_ = GetShaderLocation(simulation_shader_, "prevDepth");
    sim_time_loc_ = GetShaderLocation(shader_, "current_time");

    return prev_state_loc_ != -1 && sim_depth_texture_loc_ != -1;
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

            float time = (float)GetTime();

            SetShaderValueTexture(shader_, texture_loc_, texture_);
            SetShaderValue(shader_, current_time_, &time, SHADER_UNIFORM_FLOAT);

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

void renderer::render(DepthDataFloat depth_data) {

    if(!shader_loaded_ || !texture_loaded_)
        return;

    update_texture(depth_data);

    int read_buffer = current_sim_buffer_;
    int write_buffer = 1 - current_sim_buffer_;

    // Simulation
    BeginTextureMode(texture_buffers[write_buffer]);
        BeginShaderMode(simulation_shader_);
            float time = (float)GetTime();

            Texture2D read = texture_buffers[read_buffer].texture;
            SetShaderValue(simulation_shader_, current_time_, &time, SHADER_UNIFORM_FLOAT);
            SetShaderValueTexture(simulation_shader_, prev_state_loc_, read);
            SetShaderValueTexture(simulation_shader_, sim_depth_texture_loc_, texture_);
            SetShaderValueTexture(simulation_shader_, prev_depth_texture_loc_, prev_texture_.texture);
            DrawRectangle(0,0,read.width,read.height,WHITE);

        EndShaderMode();
    EndTextureMode();


    BeginDrawing();
            ClearBackground(BLUE);
            BeginShaderMode(shader_);

                Texture2D current = texture_buffers[write_buffer].texture;
                //ripple state
                SetShaderValueTexture(shader_, texture_loc_, texture_);
                SetShaderValueTexture(shader_, ripple_state_texture_loc_, current);
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

    current_sim_buffer_ = write_buffer;

    BeginTextureMode(prev_texture_);
        DrawTexture(texture_, 0, 0, WHITE);
    EndTextureMode();
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