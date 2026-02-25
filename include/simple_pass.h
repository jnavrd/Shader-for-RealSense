//
// Created by juana on 24/2/2026.
//

#ifndef CUSTOMSHADERSRS_SIMPLE_PASS_H
#define CUSTOMSHADERSRS_SIMPLE_PASS_H


#include "render_pass.h"

class simple_pass : public render_pass{

public:
    simple_pass(const std::string& path);

    bool init() override;

    void render(Texture2D texture) override;

    bool load_shader_uniforms() override;

    void unload_shaders() override;

};


#endif //CUSTOMSHADERSRS_SIMPLE_PASS_H
