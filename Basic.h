// Created by ari on 2/22/26.

#pragma once

#include <cstdint>
#include <string_view>

namespace Basic {
    struct Vec2 {
        union {
            struct {float x,y;};
            float xy[2];
        };
    };

    struct Vec3 {
        union {
            struct {float x,y,z;};
            float xyz[3];
        };
    };

    struct Vec4 {
        union {
            struct {float x,y,z,w;};
            struct {float r,g,b,a;};
            float xyzw[4];
        };
    };

    using Color = Vec4;

    struct Mat4 {
        union {
            struct {float m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33;};
            float m[4][4];
        };

        static Mat4 identity();
        static Mat4 translate(float x, float y, float z);
        static Mat4 scale(float x, float y, float z);
        static Mat4 rotateZ(float angle);
        static Mat4 projection(float x, float y, float width, float height);

        void debugPrint() const;

        Mat4 operator+(const Mat4 &mat4) const;
        Mat4 operator*(const Mat4 &mat4) const;
        Vec4 operator*(const Vec4 &vec4) const;
    };

    Basic::Color hexColor(uint32_t hex);
    std::string readEntireFile(std::string_view fileName);
}
