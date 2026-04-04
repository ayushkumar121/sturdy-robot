// Created by ari on 2/22/26.

#include "Texture.h"

#include <stdexcept>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glad/gl.h>

Texture::Texture(std::string_view texturePath) {
    std::cerr << "Loading texture: " << texturePath << std::endl;

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // load and generate the texture
    int nrChannels;
    unsigned char *data = stbi_load(texturePath.data(), &width, &height, &nrChannels, 0);
    if (!data) {
        throw std::runtime_error("failed to load texture");
    }
    std::cerr << "Loaded Size: " << width << " " << height << std::endl;

    auto format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
}

Texture::~Texture() {
    glDeleteTextures(1, &textureId);
}

void Texture::bind(uint32_t slot) const {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, textureId);
}

int Texture::getWidth() const {
    return width;
}

int Texture::getHeight() const {
    return height;
}
