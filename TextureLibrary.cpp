// Created by ari on 2/23/26.

#include "TextureLibrary.h"

TextureLibrary & TextureLibrary::getInstance() {
    static TextureLibrary textureLibrary;
    return textureLibrary;
}

Texture& TextureLibrary::getTexture(std::string texturePath) {
    auto it = textures.find(texturePath);
    if (it != textures.end()) {
        return it->second;
    }

    auto [insertIt, _] =
        textures.try_emplace(texturePath, texturePath);

    return insertIt->second;
}
