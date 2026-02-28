// Created by ari on 2/23/26.

#pragma once

#include <vector>

#include "Basic.h"
#include "Texture.h"

class Renderer {
public:
    struct Quad {
        Basic::Vec4 rect;
        Basic::Color color;
        Texture* texture;
    };

    void begin(Basic::Vec2 frameSize);
    void submit(Quad quad);
    void end();
private:
    std::vector<Quad> drawList;
    Basic::Mat4 projection{};
};
