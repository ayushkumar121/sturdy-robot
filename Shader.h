// Created by ari on 2/22/26.

#pragma once

#include <string_view>

#include "Basic.h"

class Shader {
public:
    Shader(std::string_view vertexPath, std::string_view fragmentPath);

    void bind() const;
    void setValue(std::string_view name, Basic::Vec4 vec4) const;
    void setValue(std::string_view name, float value) const;
    void setValue(std::string_view name, int value) const;
    void setValue(std::string_view name, Basic::Mat4 mat4) const;
private:
    unsigned int shaderId;
};
