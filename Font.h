// Created by ari on 2/24/26.

#pragma once

#include <cstdint>
#include <string_view>
#include <unordered_map>

#include "Basic.h"

class Font {
public:
	struct Face {
	    uint32_t textureId;
	    uint32_t width;
	    uint32_t height;
	    int bearingX;
	    int bearingY;
	    long advance;
	};

	Font(std::string_view fontPath, uint32_t size);
	~Font();

	uint32_t getSize() const;

	float getLineSpacing() const;
	void setLineSpacing(float lineSpacing);
	const Face& getFace(char ch) const;

	Basic::Vec2 measureText(std::string_view text) const;
private:
	uint32_t size;
	float lineSpacing = 1.25f;
	std::unordered_map<char, Face> faces;
};