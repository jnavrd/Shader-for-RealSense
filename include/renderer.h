#ifndef CUSTOMSHADERSRS_RENDERER_H
#define CUSTOMSHADERSRS_RENDERER_H

#include <vector>
#include "raylib.h"
#include "depth_converter.h"

class renderer {
public:
    renderer(int window_width, int window_height, const char *title);
    ~renderer();

    void init();
    void update_texture(GrayscaleImg& img);
    void render();
    bool should_close() const;  // wrapper de WindowShouldClose
    void close();

private:
    int window_width_;
    int window_height_;
    const char *title_;
    Texture2D texture_;  // textura activa en GPU
    bool texture_loaded_;
};
#endif //CUSTOMSHADERSRS_renderer_H