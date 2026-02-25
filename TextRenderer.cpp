// Created by ari on 2/24/26.

#include <glad/gl.h>

#include "TextRenderer.h"
#include "ShaderLibrary.h"
#include "QuadMesh.h"

void TextRenderer::begin(Basic::Vec4 rect) {
	this->projection = Basic::Mat4::projection(rect.x, rect.y, rect.z, rect.w);
	drawList.clear();
}

void TextRenderer::submit(Glyph glyph) {
	drawList.push_back(glyph);
}

void TextRenderer::end() {
    Shader& shader = ShaderLibrary::getInstance().getShader(ShaderLibrary::ShaderType::FONT);
    shader.bind();
    shader.setValue("view", Basic::Mat4::identity());
    shader.setValue("projection", projection);
    shader.setValue("tex", 0);

    for (const Glyph& glyph : drawList) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, glyph.textureId);

        auto transform = Basic::Mat4::identity()
            * Basic::Mat4::translate(glyph.pos.x, glyph.pos.y, 0.0f)
            * Basic::Mat4::scale(glyph.size.x, glyph.size.y, 1.0f);

        shader.setValue("transform", transform);
        shader.setValue("color", glyph.color);

        QuadMesh::getInstance().draw();
    }
}