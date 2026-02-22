//
// Created by ari on 2/23/26.
//

#include "Renderer.h"

#include <exception>

#include "QuadMesh.h"
#include "ShaderLibrary.h"

void Renderer::begin(const Basic::Mat4 &projectionMat) {
    this->projection = projectionMat;
    drawList.clear();
}

void Renderer::submit(Quad &quad) {
    drawList.push_back(quad);
}

void Renderer::end() const {
    Shader& shader = ShaderLibrary::getInstance().getShader(ShaderLibrary::ShaderType::TEXTURED);
    shader.bind();
    shader.setValue("view", Basic::Mat4::identity());
    shader.setValue("projection", projection);
    shader.setValue("tex", 0);

    Texture* currentTexture = nullptr;
    for (const Quad& quad : drawList) {
        if (quad.texture != currentTexture) {
            currentTexture = quad.texture;
            if (currentTexture) {
                shader.setValue("useTexture", 1);
                currentTexture->bind(0);
            } else {
                shader.setValue("useTexture", 0);
            }
        }

        auto transform = Basic::Mat4::identity()
            * Basic::Mat4::translate(quad.x, quad.y, 0.0f)
            * Basic::Mat4::scale(quad.width, quad.height, 1.0f);

        shader.setValue("transform", transform);
        shader.setValue("color", quad.color);

        QuadMesh::getInstance().draw();
    }
}
