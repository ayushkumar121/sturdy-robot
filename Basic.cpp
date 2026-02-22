//
// Created by ari on 2/22/26.
//

#include "Basic.h"

#include <format>
#include <fstream>
#include <iostream>

Basic::Mat4 Basic::Mat4::identity() {
    Mat4 result{};
    result.m00 = 1.0f;
    result.m11 = 1.0f;
    result.m22 = 1.0f;
    result.m33 = 1.0f;
    return result;
}

Basic::Mat4 Basic::Mat4::operator+(const Mat4 &mat4) const {
    Mat4 result{};
    for (unsigned int i = 0; i < 4; ++i) {
        for (unsigned int j = 0; j < 4; ++j) {
            result.m[i][j] = this->m[i][j] + mat4.m[i][j];
        }
    }
    return result;
}

Basic::Mat4 Basic::Mat4::operator*(const Mat4 &mat4) const {
    Mat4 result{};
    for (unsigned int i = 0; i < 4; ++i) {
        for (unsigned int j = 0; j < 4; ++j) {
            float sum = 0.0f;
            for (unsigned int k = 0; k < 4; ++k) {
                sum += this->m[i][k] * mat4.m[k][j];
            }
            result.m[i][j] = sum;
        }
    }
    return result;
}

Basic::Vec4 Basic::Mat4::operator*(const Vec4 &vec4) const {
    return Vec4{
        .x = m00 * vec4.x + m01 * vec4.y + m02 * vec4.z + m03 * vec4.w,
        .y = m10 * vec4.x + m11 * vec4.y + m12 * vec4.z + m13 * vec4.w,
        .z = m20 * vec4.x + m21 * vec4.y + m22 * vec4.z + m23 * vec4.w,
        .w = m30 * vec4.x + m31 * vec4.y + m32 * vec4.z + m33 * vec4.w
    };
}

std::string Basic::readEntireFile(std::string_view fileName) {
    std::ifstream file(fileName.data());
    if (!file) {
        throw std::runtime_error(std::format("File {} not found", fileName));
    }

    file.seekg(0, std::ios::end);
    std::string content(file.tellg(), '\0');
    file.seekg(0, std::ios::beg);
    file.read(content.data(), content.size());

    return content;
}
