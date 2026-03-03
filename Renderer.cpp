// Created by ari on 2/23/26.

#include "Renderer.h"
#include "QuadMesh.h"
#include "WhiteTexture.h"
#include "ShaderLibrary.h"

void Renderer::begin(Basic::Vec2 frameSize) {
    this->projection = Basic::Mat4::projection(0.0f, 0.0f, frameSize.x, frameSize.y);
    drawList.clear();
}

void Renderer::submit(Quad quad) {
    drawList.push_back(quad);
}

void Renderer::end() {
    Shader& shader = ShaderLibrary::getInstance().getShader(ShaderLibrary::ShaderType::QUAD);
    shader.bind();
    shader.setValue("view", Basic::Mat4::identity());
    shader.setValue("projection", projection);
    shader.setValue("tex", 0);

    WhiteTexture& whiteTexture = WhiteTexture::getInstance();

    for (const Quad& quad : drawList) {
        int width, height;
        if (quad.texture != nullptr) {
            quad.texture->bind(0);
            width = quad.texture->getWidth();
            height = quad.texture->getWidth();
        } else {
            whiteTexture.bind(0);
            width = 1;
            height = 1;
        }

        auto transform = Basic::Mat4::identity()
            * Basic::Mat4::translate(quad.rect.x, quad.rect.y, 0.0f)
            * Basic::Mat4::scale(quad.rect.z, quad.rect.w, 1.0f);

        shader.setValue("transform", transform);
        shader.setValue("color", quad.color);
        shader.setValue("borderColor", Basic::hexColor(0xFF000000));
        shader.setValue("borderSize", 4.0f);
        shader.setValue("rectSize", Basic::Vec2{quad.rect.z, quad.rect.w});
        shader.setValue("texelSize", Basic::Vec2{1.0f/(float)width, 1.0f/(float)height});

        QuadMesh::getInstance().draw();
    }
}
