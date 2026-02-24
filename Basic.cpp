//
// Created by ari on 2/22/26.
//

#include "Basic.h"

#include <cmath>
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

Basic::Mat4 Basic::Mat4::translate(float x, float y, float z) {
    Mat4 result = identity();

    result.m03 = x;
    result.m13 = y;
    result.m23 = z;

    return result;
}

Basic::Mat4 Basic::Mat4::scale(float x, float y, float z) {
    Mat4 result = identity();

    result.m00 = x;
    result.m11 = y;
    result.m22 = z;

    return result;
}

Basic::Mat4 Basic::Mat4::rotateZ(float angle) {
    Mat4 result = identity();

    const float c = std::cos(angle);
    const float s = std::sin(angle);

    result.m00 = c;
    result.m01 = -s;
    result.m10 = s;
    result.m11 = c;

    return result;
}

Basic::Mat4 Basic::Mat4::projection(float x, float y, float width, float height) {
    float left = x;
    float right = width;
    float top = y;
    float bottom = height;
    float nearZ = -1.0f;
    float farZ = 1.0f;

    Mat4 result = identity();
    result.m00 = 2.0f / (right - left);
    result.m11 = 2.0f/ (top - bottom);
    result.m22 = -2.0f / (farZ - nearZ);

    result.m03 = -(right + left) / (right - left);
    result.m13 = -(top + bottom) / (top - bottom);
    result.m23 = -(farZ + nearZ) / (farZ - nearZ);

    return result;
}

void Basic::Mat4::debugPrint() const {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            std::cout << this->m[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

Basic::Mat4 Basic::Mat4::operator+(const Mat4 &mat4) const {
    Mat4 result{};
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            result.m[i][j] = this->m[i][j] + mat4.m[i][j];
        }
    }
    return result;
}

Basic::Mat4 Basic::Mat4::operator*(const Mat4 &mat4) const {
    Mat4 result{};
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            float sum = 0.0f;
            for (int k = 0; k < 4; ++k) {
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

Basic::Color Basic::hexColor(uint32_t hex) {
    float b = (float)(hex & 0xFF)/255.0f;
    float g = (float)((hex >> 8) & 0xFF)/255.0f;
    float r = (float)((hex >> 16) & 0xFF)/255.0f;
    float a = (float)((hex >> 24) & 0xFF)/255.0f;
    return Vec4{r, g, b, a};    
}
