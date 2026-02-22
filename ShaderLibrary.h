//
// Created by ari on 2/22/26.
//

#ifndef STURDY_ROBOT_SHADERLIBRARY_H
#define STURDY_ROBOT_SHADERLIBRARY_H

#include <unordered_map>

#include "Shader.h"

class ShaderLibrary {
public:
    enum class ShaderType {
        COLORED,
        TEXTURED
    };

    ~ShaderLibrary();
    ShaderLibrary(const ShaderLibrary&) = delete;
    ShaderLibrary& operator=(const ShaderLibrary&) = delete;

    static ShaderLibrary& getInstance();
    Shader& getShader(ShaderType type);
private:
    ShaderLibrary();
    std::unordered_map<ShaderType, Shader> shaders;
};


#endif //STURDY_ROBOT_SHADERLIBRARY_H