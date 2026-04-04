// Created by ari on 3/3/26.

#pragma once
#include <cstdint>

class DummyTexture {
public:
    static DummyTexture& getInstance();
    ~DummyTexture();
    void bind(uint32_t slot) const;
    static const int width = 1;
    static const int height = 1;
private:
    DummyTexture();
    uint32_t textureId{};
};
