// Created by ari on 2/24/26.

#include "Font.h"

#include <iostream>
#include <stdexcept>

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>
#include <glad/gl.h>

Font::Font(std::string_view fontPath, uint32_t size) : size(size) {
	std::string fileData = Basic::readEntireFile(fontPath);

    fontBuffer.assign(fileData.begin(), fileData.end());

    if (!stbtt_InitFont(
            &fontInfo,
            reinterpret_cast<const unsigned char*>(fontBuffer.data()),
            stbtt_GetFontOffsetForIndex(
                reinterpret_cast<const unsigned char*>(fontBuffer.data()), 0))) {
        throw std::runtime_error("failed initialize stb_truetype");
    }

    scale = stbtt_ScaleForPixelHeight(&fontInfo, (float)size);

	// FT_Library ft;
	// if (FT_Init_FreeType(&ft)) {
	// 	throw std::runtime_error("failed initialize freeType");
	// }

	// FT_Face face;
	// if (FT_New_Face(ft, fontPath.data(), 0, &face)) {
	// 	throw std::runtime_error("failed to load font");
	// }

	// FT_Set_Pixel_Sizes(face, 0, size);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (unsigned char c = 0; c < 128; c++) {
	  	int width, height, xoff, yoff;

        unsigned char* bitmap = stbtt_GetCodepointBitmap(&fontInfo, 0, scale, c, &width, &height, &xoff, &yoff);

        int advance, leftBearing;
        stbtt_GetCodepointHMetrics(&fontInfo, c, &advance, &leftBearing);

		// If the glyph is not renderable, skip uploading to GPU
		if (width == 0 || height == 0) {
			Face f{0, 0, 0, leftBearing, 0, (int)(advance * scale * 64.0f)};
			faces.insert({c, f});
			continue;
		}

	    uint32_t texture;
	    glGenTextures(1, &texture);
	    glBindTexture(GL_TEXTURE_2D, texture);
	    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, bitmap);

	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	    Face f{texture, (uint32_t)width, (uint32_t)height, xoff, -yoff, (int)(advance * scale * 64.0f)};
		
	    faces.insert({c, f});

	    stbtt_FreeBitmap(bitmap, nullptr);
	}
}

Font::~Font(){
	std::cout << "Font deleted" << std::endl;
	for (auto& [_, face] : faces) {
		glDeleteTextures(1, &face.textureId);
	}
}

uint32_t Font::getSize() const {
	return size;
}

float Font::getLineSpacing() const {
	return lineSpacing;
}

void Font::setLineSpacing(float lineSpacing) {
	this->lineSpacing = lineSpacing;
}

const Font::Face& Font::getFace(char ch) const {
	return faces.at(ch); 
}

Basic::Vec2 Font::measureText(std::string_view text) const {
	float offsetX = 0.0f;
    float maxX = 0.0f;
    float maxBearingY = 0.0f;

    for (auto& ch : text) {
        const Font::Face& face = getFace(ch);
        if (ch == '\n') {
            offsetX = 0.0f;
            continue;
        }

        maxBearingY = std::max(maxBearingY, (float)face.bearingY);
        offsetX += face.advance >> 6;
        maxX = std::max(maxX, offsetX);
    }

    return {maxX, maxBearingY};
}