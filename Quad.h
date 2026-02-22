//
// Created by ari on 2/22/26.
//

#ifndef STURDY_ROBOT_QUAD_H
#define STURDY_ROBOT_QUAD_H

#include "Basic.h"
#include "Texture.h"

class Quad {
public:
    Basic::Vec4 color;
    Texture* texture;
    float x, y, width, height;

    Quad(float x, float y, float width, float height, Basic::Vec4 color, Texture* texture);
};

#endif //STURDY_ROBOT_QUAD_H