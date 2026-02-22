//
// Created by ari on 2/22/26.
//

#include "Quad.h"

Quad::Quad(float x, float y, float width, float height, Basic::Vec4 color, Texture *texture)
    : color(color),
      texture(texture),
      x(x),
      y(y),
      width(width),
      height(height) {
}
