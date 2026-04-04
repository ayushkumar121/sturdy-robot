// Created by ari on 2/22/26.

#include "Shader.h"

#include <glad/gl.h>
#include <iostream>

#include "Basic.h"

Shader::Shader(std::string_view vertexPath, std::string_view fragmentPath) {
    int success;
    char infoLog[512];

    auto vertexShaderSource = Basic::readEntireFile(vertexPath);
    auto fragmentShaderSource = Basic::readEntireFile(fragmentPath);

    const char* vertexShaderSourceArray = vertexShaderSource.c_str();
    const char* fragmentShaderSourceArray = fragmentShaderSource.c_str();

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSourceArray, nullptr);
    glCompileShader(vertexShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, sizeof(infoLog), nullptr, infoLog);
        std::cerr << "ERROR:Vertex Shader compilation failed: " << infoLog << std::endl;
    }

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSourceArray, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, sizeof(infoLog), nullptr, infoLog);
        std::cerr << "ERROR:Fragment Shader compilation failed: " << infoLog << std::endl;
    }

    shaderId = glCreateProgram();
    glAttachShader(shaderId, vertexShader);
    glAttachShader(shaderId, fragmentShader);
    glLinkProgram(shaderId);

    glGetProgramiv(shaderId, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderId, 512, nullptr, infoLog);
        std::cerr << "ERROR:Program linking failed: " << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    std::cerr << "Shader program linking success: " << success << std::endl;
}

void Shader::bind() const {
    glUseProgram(shaderId);
}

void Shader::setValue(std::string_view name, Basic::Vec4 vec4) const {
    glUniform4f(glGetUniformLocation(shaderId, name.data()), vec4.x, vec4.y, vec4.z, vec4.w);
}

void Shader::setValue(std::string_view name, Basic::Vec2 vec2) const {
    glUniform2f(glGetUniformLocation(shaderId, name.data()), vec2.x, vec2.y);
}

void Shader::setValue(std::string_view name, float value) const {
    glUniform1f(glGetUniformLocation(shaderId, name.data()), value);
}

void Shader::setValue(std::string_view name, int value) const {
    glUniform1i(glGetUniformLocation(shaderId, name.data()), value);
}

void Shader::setValue(std::string_view name, Basic::Mat4 mat4) const {
    glUniformMatrix4fv(glGetUniformLocation(shaderId, name.data()), 1, GL_TRUE, &mat4.m00);
}
