// Created by ari on 2/22/26.

#pragma once

#include <stdint.h>
#include <string>

class Texture {
public:
    Texture(std::string texturePath);
    ~Texture();
    void bind(uint32_t slot) const;
    int getWidth() const;
    int getHeight() const;
private:
    int width;
    int height;
    std::string texturePath;
    uint32_t textureId;
};
