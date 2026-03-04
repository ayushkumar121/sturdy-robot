// Created by ari on 2/24/26.

#include <glad/gl.h>
#include <cassert>

#include "TextRenderer.h"
#include "FontLibrary.h"
#include "ShaderLibrary.h"
#include "QuadMesh.h"

void TextRenderer::begin(Basic::Vec2 frameSize) {
	this->projection = Basic::Mat4::projection(0.0f, 0.0f, frameSize.x, frameSize.y);
	drawList.clear();
}

void TextRenderer::submit(Text text) {
	drawList.push_back(text);
}

void TextRenderer::end() {
    const Shader& shader = ShaderLibrary::getInstance().getShader(ShaderLibrary::ShaderType::FONT);
    shader.bind();
    shader.setValue("view", Basic::Mat4::identity());
    shader.setValue("projection", projection);
    shader.setValue("tex", 0);

    for (const Text& text : drawList) {
    	assert(text.font != nullptr);
	    float offsetX = 0.0f;
	    float offsetY = 0.0f;

	    for (auto& ch: text.data) {
	    	const Font::Face& face = text.font->getFace(ch);
			if (face.textureId == 0) {
				offsetX += face.advance >> 6;
				continue;
			}
			
	    	if (ch == '\n') {
	    		offsetY += (face.height + face.bearingY) * text.font->getLineSpacing();
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
