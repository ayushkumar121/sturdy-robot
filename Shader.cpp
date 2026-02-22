//
// Created by ari on 2/22/26.
//

#include "Shader.h"

#include <glad/gl.h>
#include <fstream>
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
        std::cout << "Vertex Shader compilation failed: " << infoLog << std::endl;
    }

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSourceArray, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, sizeof(infoLog), nullptr, infoLog);
        std::cout << "Fragment Shader compilation failed: " << infoLog << std::endl;
    }

    shaderId = glCreateProgram();
    glAttachShader(shaderId, vertexShader);
    glAttachShader(shaderId, fragmentShader);
    glLinkProgram(shaderId);

    glGetProgramiv(shaderId, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderId, 512, nullptr, infoLog);
        std::cout << "Program linking failed: " << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::use() const {
    glUseProgram(shaderId);
}

void Shader::setValue(std::string_view name, float x, float y, float z, float w) const {
    glUniform4f(glGetUniformLocation(shaderId, name.data()), x, y, z, w);
}

void Shader::setValue(std::string_view name, float value) const {
    glUniform1f(glGetUniformLocation(shaderId, name.data()), value);
}

void Shader::setValue(std::string_view name, int value) const {
    glUniform1i(glGetUniformLocation(shaderId, name.data()), value);
}
