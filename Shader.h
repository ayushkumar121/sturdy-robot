//
// Created by ari on 2/22/26.
//

#ifndef STURDY_ROBOT_SHADER_H
#define STURDY_ROBOT_SHADER_H
#include <string_view>

#include "Basic.h"

class Shader {
public:
    Shader(std::string_view vertexPath, std::string_view fragmentPath);

    void bind() const;
    void setValue(std::string_view name, float x, float y, float z, float w) const;
    void setValue(std::string_view name, float value) const;
    void setValue(std::string_view name, int value) const;
    void setValue(std::string_view name, Basic::Mat4 mat4) const;
private:
    unsigned int shaderId;
};


#endif //STURDY_ROBOT_SHADER_H