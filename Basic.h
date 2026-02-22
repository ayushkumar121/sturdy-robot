//
// Created by ari on 2/22/26.
//

#ifndef STURDY_ROBOT_BASIC_H
#define STURDY_ROBOT_BASIC_H

#include <cmath>
#include <string_view>

namespace Basic {
    struct Vec4 {
        union {
            struct {float x,y,z,w;};
            float xyzw[4];
        };

        [[nodiscard]] float squaredLength() const {
            return xyzw[0]*xyzw[0] + xyzw[1]*xyzw[1] + xyzw[2]*xyzw[2];
        }

        [[nodiscard]] float length() const {
            return sqrtf(squaredLength());
        }
    };

    struct Mat4 {
        union {
            struct {float m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33;};
            float m[4][4];
        };

        static Mat4 identity();

        Mat4 operator+(const Mat4 &mat4) const;
        Mat4 operator*(const Mat4 &mat4) const;
        Vec4 operator*(const Vec4 &vec4) const;
    };

    std::string readEntireFile(std::string_view fileName);
}


#endif //STURDY_ROBOT_BASIC_H