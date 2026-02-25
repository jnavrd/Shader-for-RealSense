#ifndef CUSTOMSHADERSRS_RENDERER_H
#define CUSTOMSHADERSRS_RENDERER_H

#include <memory>
#include "raylib.h"
#include "frame_types.h"
#include "render_pass.h"

class renderer {
public:
    renderer(int window_width, int window_height, const char *title);
    ~renderer() = default;

    bool init();

    void update_texture(DepthDataFloat depth_data);

    void set_pass(std::unique_ptr<render_pass> pass);

    void set_shader_params(float min, float max);

    void render(DepthDataFloat depth_data);

    static bool should_close() ;  // wrapper de WindowShouldClose
    void close();

private:
    //window
    int window_width_;
    int window_height_;
    const char *title_;

    Texture2D texture_; // textura activa en GPU

    std::unique_ptr<render_pass> render_pass_; //unique pointer, para eliminar el pass en caso de cambiar de tipo de pass

    //flags
    bool texture_loaded_;

};

#endif //CUSTOMSHADERSRS_renderer_H