// Created by ari on 2/22/26.

#pragma once

#include <cstdint>
#include <string_view>

class Texture {
public:
    Texture(std::string_view texturePath);
    ~Texture();
    void bind(uint32_t slot) const;
    int getWidth() const;
    int getHeight() const;
private:
    int width{};
    int height{};
    uint32_t textureId{};
};
