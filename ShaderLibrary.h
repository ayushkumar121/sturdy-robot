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
        TEXTURED
    };

    ShaderLibrary();

    Shader& getShader(ShaderType type);
private:
    std::unordered_map<ShaderType, Shader> shaders;
};


#endif //STURDY_ROBOT_SHADERLIBRARY_H