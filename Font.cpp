// Created by ari on 2/24/26.

#include "Font.h"

#include <iostream>
#include <stdexcept>

#include <glad/gl.h>
#include <ft2build.h>
#include FT_FREETYPE_H

Font::Font(std::string_view fontPath, uint32_t size) {
	FT_Library ft;
	if (FT_Init_FreeType(&ft)) {
		throw std::runtime_error("failed initilize freeType");
	}

	FT_Face face;
	if (FT_New_Face(ft, fontPath.data(), 0, &face)) {
		throw std::runtime_error("failed to load font");
	}
	
	FT_Set_Pixel_Sizes(face, 0, size);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (unsigned char c = 0; c < 128; c++) {
	    if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
			std::cout << "Failed to load glyph for " << c << std::endl;
			continue;
	    }

		// If the glyph is not renderable, skip uploading to GPU
		if (face->glyph->bitmap.width == 0 || face->glyph->bitmap.rows == 0) {
			Face f{0, 0, 0, face->glyph->bitmap_left, face->glyph->bitmap_top, face->glyph->advance.x};
			faces.insert({c, f});
			continue;
		}

	    uint32_t texture;
	    glGenTextures(1, &texture);
	    glBindTexture(GL_TEXTURE_2D, texture);
	    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	    Face f{
	    	texture,
	        face->glyph->bitmap.width, 
	        face->glyph->bitmap.rows,
	        face->glyph->bitmap_left, 
	        face->glyph->bitmap_top,
	        face->glyph->advance.x
	    };
		
	    faces.insert({c, f});
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