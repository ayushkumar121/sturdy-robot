// Created by ari on 2/23/26.

#pragma once

#include <string>
#include <unordered_map>

#include "Texture.h"

class TextureLibrary {
public:
    ~TextureLibrary();
    static TextureLibrary& getInstance();
    Texture& getTexture(std::string texturePath);
private:
    std::unordered_map<std::string, Texture> textures;
};
