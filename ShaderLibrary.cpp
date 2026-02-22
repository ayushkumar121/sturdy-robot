//
// Created by ari on 2/22/26.
//

#include "ShaderLibrary.h"

ShaderLibrary::ShaderLibrary() {
    shaders.emplace(
        ShaderType::TEXTURED,
        Shader("assets/shaders/basic.vert", "assets/shaders/textured.frag")
    );
}

Shader& ShaderLibrary::getShader(const ShaderType type) {
    return shaders.at(type);
}
