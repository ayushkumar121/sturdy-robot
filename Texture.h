//
// Created by ari on 2/22/26.
//

#ifndef STURDY_ROBOT_TEXTURE_H
#define STURDY_ROBOT_TEXTURE_H
#include <string_view>


class Texture {
public:
    Texture(std::string_view texturePath);
    void bind(unsigned int slot) const;
private:
    unsigned int textureId;
};


#endif //STURDY_ROBOT_TEXTURE_H