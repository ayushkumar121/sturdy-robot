//
// Created by ari on 2/23/26.
//

#include "TextureLibrary.h"

#include <iostream>

TextureLibrary::TextureLibrary() {
    std::cout << "Loading textures" << std::endl;

    std::string_view texturePaths[] = {
        "assets/wall.jpg",
        "assets/awesomeface.png",
    };

    for (const auto& texturePath : texturePaths) {
        textures.emplace(texturePath, Texture(texturePath));
    }
}

TextureLibrary & TextureLibrary::getInstance() {
    static TextureLibrary textureLibrary;
    return textureLibrary;
}

Texture& TextureLibrary::getTexture(const std::string_view texturePath) {
    return textures.at(texturePath);
}
