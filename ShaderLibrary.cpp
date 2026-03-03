// Created by ari on 2/22/26.

#include "ShaderLibrary.h"

#include <iostream>
#include <ostream>

ShaderLibrary::ShaderLibrary() {
    std::cout << "Compiling shaders" << std::endl;
    shaders.emplace(
        ShaderType::QUAD,
        Shader("assets/shaders/basic.vert", "assets/shaders/quad.frag")
    );
    shaders.emplace(
        ShaderType::FONT,
        Shader("assets/shaders/basic.vert", "assets/shaders/font.frag")
    );
}

ShaderLibrary::~ShaderLibrary() {
    std::cout << "Shader Library Destroyed" << std::endl;
}

ShaderLibrary& ShaderLibrary::getInstance() {
    static ShaderLibrary shaderLibrary;
    return shaderLibrary;
}

Shader& ShaderLibrary::getShader(const ShaderType type) {
    return shaders.at(type);
}
