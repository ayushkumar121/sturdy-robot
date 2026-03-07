// Created by ari on 2/23/26.

#include "Renderer.h"

#include "DummyTexture.h"
#include "QuadMesh.h"
#include "ShaderLibrary.h"
#include "Camera.h"

void Renderer::begin(Basic::Vec2 frameSize) {
    this->projection = Basic::Mat4::projection(0.0f, 0.0f, frameSize.x, frameSize.y);
    drawList.clear();
}

void Renderer::submit(Quad quad) {
    drawList.push_back(quad);
}

void Renderer::end() {
    const Shader& shader = ShaderLibrary::getInstance().getShader(ShaderLibrary::ShaderType::QUAD);
    shader.bind();
    shader.setValue("view", Camera::getInstance().transform());
    shader.setValue("projection", projection);
    shader.setValue("tex", 0);

    for (const Quad& quad : drawList) {
        if (quad.texture != nullptr) {
            quad.texture->bind(0);
        } else {
            DummyTexture::getInstance().bind(0);
        }

        auto transform = Basic::Mat4::identity()
            * Basic::Mat4::translate(quad.rect.x, quad.rect.y, 0.0f)
            * Basic::Mat4::scale(quad.rect.z, quad.rect.w, 1.0f);

        shader.setValue("transform", transform);
        shader.setValue("color", quad.color);

        QuadMesh::getInstance().draw();
    }
}
