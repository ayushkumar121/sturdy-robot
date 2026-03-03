#include "WhiteTexture.h"
#include <iostream>
#include <glad/gl.h>

WhiteTexture::WhiteTexture() {
    std::cerr << "Texture Created white texture" << std::endl;

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    uint32_t data = 0xFFFFFFFF;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &data);
    glGenerateMipmap(GL_TEXTURE_2D);
}

WhiteTexture::~WhiteTexture() {
    std::cerr << "Texture Deleted white texture" << std::endl;
    glDeleteTextures(1, &textureId);
}

WhiteTexture& WhiteTexture::getInstance() {
    static WhiteTexture instance;
    return instance;
}

void WhiteTexture::bind(uint32_t slot) const {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, textureId);
}

int WhiteTexture::getWidth() const {return 1;}
int WhiteTexture::getHeight() const {return 1;}