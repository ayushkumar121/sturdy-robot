//
// Created by ari on 2/22/26.
//

#include "ShaderLibrary.h"

#include <iostream>
#include <ostream>

ShaderLibrary::ShaderLibrary() {
    std::cout << "Compiling shaders" << std::endl;
    shaders.emplace(
        ShaderType::TEXTURED,
        Shader("assets/shaders/basic.vert", "assets/shaders/textured.frag")
    );
}

ShaderLibrary::~ShaderLibrary() {
    std::cout << "Shader library destroyed" << std::endl;
}

ShaderLibrary& ShaderLibrary::getInstance() {
    static ShaderLibrary shaderLibrary;
    return shaderLibrary;
}

Shader& ShaderLibrary::getShader(const ShaderType type) {
    return shaders.at(type);
}
