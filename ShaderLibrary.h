// Created by ari on 2/22/26.

#pragma once

#include <unordered_map>

#include "Shader.h"

class ShaderLibrary {
public:
    enum class ShaderType {
        QUAD,
        FONT
    };
    static ShaderLibrary& getInstance();
    Shader& getShader(ShaderType type);
private:
    ShaderLibrary();
    std::unordered_map<ShaderType, Shader> shaders;
};
