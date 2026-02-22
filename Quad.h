//
// Created by ari on 2/22/26.
//

#ifndef STURDY_ROBOT_QUAD_H
#define STURDY_ROBOT_QUAD_H

#include "Basic.h"

class Quad {
public:
    struct Vertex {
        float pos[2];
        float uv[2];
    };

    Quad(float x, float y, float width, float height, Basic::Vec4 color);
    ~Quad();

    void bind() const;
    void draw() const;
private:
    Basic::Vec4 color;
    float x, y, width, height;

    unsigned int vao;
    unsigned int vbo;
};


#endif //STURDY_ROBOT_QUAD_H