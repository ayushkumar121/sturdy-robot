//
// Created by ari on 2/23/26.
//

#ifndef STURDY_ROBOT_TEXTURELIBRARY_H
#define STURDY_ROBOT_TEXTURELIBRARY_H

#include <string_view>
#include <unordered_map>

#include "Texture.h"


class TextureLibrary {
public:
    static TextureLibrary& getInstance();
    Texture& getTexture(std::string_view texturePath);
private:
    TextureLibrary();
    std::unordered_map<std::string_view, Texture> textures;
};


#endif //STURDY_ROBOT_TEXTURELIBRARY_H