// Created by ari on 2/24/26.

#include <glad/gl.h>

#include "TextRenderer.h"
#include "ShaderLibrary.h"
#include "QuadMesh.h"

void TextRenderer::begin(Font* font, Basic::Vec4 rect) {
	this->font = font;
	this->projection = Basic::Mat4::projection(rect.x, rect.y, rect.z, rect.w);
	drawList.clear();
}

void TextRenderer::submit(Text text) {
	drawList.push_back(text);
}

void TextRenderer::end() {
    Shader& shader = ShaderLibrary::getInstance().getShader(ShaderLibrary::ShaderType::FONT);
    shader.bind();
    shader.setValue("view", Basic::Mat4::identity());
    shader.setValue("projection", projection);
    shader.setValue("tex", 0);

    for (const Text& text : drawList) {
	    float offsetX = 0.0f;
	    float offsetY = 0.0f;
	    for (auto& ch: text.data) {
	    	const Font::Face& face = font->getFace(ch);
			if (face.textureId == 0) {
				offsetX += face.advance >> 6;
				continue;
			}
			
	    	if (ch == '\n') {
	    		offsetY += face.height + face.bearingY;
	    		offsetX = 0.0f;
	    		continue;
	    	}

	    	glActiveTexture(GL_TEXTURE0);
	   		glBindTexture(GL_TEXTURE_2D, face.textureId);

	    	auto transform = Basic::Mat4::identity()
            * Basic::Mat4::translate(offsetX + text.pos.x + face.bearingX, offsetY + text.pos.y - face.bearingY, 0.0f)
            * Basic::Mat4::scale(face.width, face.height, 1.0f);

	        shader.setValue("transform", transform);
	        shader.setValue("color", text.color);

	        offsetX += face.advance >> 6;

	        QuadMesh::getInstance().draw();
	    }
    }
}