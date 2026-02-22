//
// Created by ari on 2/23/26.
//

#include <iostream>
#include <algorithm>

#include "Renderer.h"
#include "QuadMesh.h"
#include "ShaderLibrary.h"

void Renderer::begin(const Basic::Mat4 &projectionMat) {
    this->projection = projectionMat;
    drawList.clear();
}

void Renderer::submit(Quad &quad) {
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
            * Basic::Mat4::translate(quad.x, quad.y, 0.0f)
            * Basic::Mat4::scale(quad.width, quad.height, 1.0f);

        currentShader->setValue("transform", transform);
        currentShader->setValue("color", quad.color);

        QuadMesh::getInstance().draw();
    }
}
