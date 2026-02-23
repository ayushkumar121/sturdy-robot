//
// Created by ari on 2/23/26.
//

#include "TextureLibrary.h"

#include <iostream>

TextureLibrary & TextureLibrary::getInstance() {
    static TextureLibrary textureLibrary;
    return textureLibrary;
}

Texture& TextureLibrary::getTexture(const std::string_view texturePath) {
    auto it = textures.find(texturePath);
    if (it != textures.end()) {
        return it->second;
    }

    auto [insertIt, _] =
        textures.emplace(texturePath, Texture(texturePath));

    return insertIt->second;
}
