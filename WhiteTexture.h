#pragma once

#include <stdint.h>
#include <string>

class WhiteTexture {
public:
    ~WhiteTexture();
    static WhiteTexture& getInstance();
    void bind(uint32_t slot) const;
    int getWidth() const;
    int getHeight() const;
private:
    WhiteTexture();
    uint32_t textureId{};
};
