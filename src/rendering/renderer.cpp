
#include "renderer.h"


renderer::renderer(int window_width, int window_height, const char *title) :
window_height_(window_height), window_width_(window_width), title_(title), texture_loaded_(false) {

}

void renderer::init() {
    InitWindow(window_width_, window_height_, title_);
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

void renderer::render() {
    BeginDrawing();
        ClearBackground(BLACK);
        DrawTexture(texture_, 0, 0, WHITE);
    EndDrawing();
}

bool renderer::should_close() const {

    return WindowShouldClose();
}

void renderer::close() {
    UnloadTexture(texture_);
    CloseWindow();
}

renderer::~renderer() = default;