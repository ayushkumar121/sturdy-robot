// Created by ari on 2/23/26.

#include "Renderer.h"
#include "QuadMesh.h"
#include "ShaderLibrary.h"

void Renderer::begin(Basic::Vec2 frameSize) {
    this->projection = Basic::Mat4::projection(0.0f, 0.0f, frameSize.x, frameSize.y);
    drawList.clear();
}

void Renderer::submit(Quad quad) {
    drawList.push_back(quad);
}

void Renderer::end() {
    auto& shaderLib = ShaderLibrary::getInstance();
    Shader* currentShader = nullptr;
    Texture* currentTexture = nullptr;

    for (const Quad& quad : drawList) {
        Shader* needed = quad.texture
        ? &shaderLib.getShader(ShaderLibrary::ShaderType::TEXTURED)
        : &shaderLib.getShader(ShaderLibrary::ShaderType::COLORED);

        if (needed != currentShader) {
            currentShader = needed;
            currentShader->bind();
            currentShader->setValue("view", Basic::Mat4::identity());
            currentShader->setValue("projection", projection);
            if (quad.texture) {
                currentShader->setValue("tex", 0);
            }
        }

        if (quad.texture != currentTexture) {
            currentTexture = quad.texture;
            if (currentTexture) {
                currentTexture->bind(0);
            }
        }

        auto transform = Basic::Mat4::identity()
            * Basic::Mat4::translate(quad.rect.x, quad.rect.y, 0.0f)
            * Basic::Mat4::scale(quad.rect.z, quad.rect.w, 1.0f);

        currentShader->setValue("transform", transform);
        currentShader->setValue("color", quad.color);

        QuadMesh::getInstance().draw();
    }
}
