//
// Created by ari on 2/22/26.
//

#include "Quad.h"

#include <cstddef>

#include "GameContext.h"
#include "glad/gl.h"

#include "Shader.h"
#include "ShaderLibrary.h"

Quad::Quad(float x, float y, float width, float height, Basic::Vec4 color): color(color), x(x), y(y), width(width), height(height) {
    static Vertex vertices[] = {
        {0.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f, 1.0f},
        {1.0f, 0.0f, 1.0f, 0.0f},
        {1.0f, 0.0f, 1.0f, 0.0f},
        {0.0f, 1.0f, 0.0f, 1.0f},
        {1.0f, 1.0f, 1.0f, 1.0f}
    };

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
    glEnableVertexAttribArray(1);
}

Quad::~Quad() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}

void Quad::bind() const {
    // Setup Calls
    auto transform = Basic::Mat4::identity() * Basic::Mat4::translate(x, y, 0.0f) * Basic::Mat4::scale(width, height, 1.0f);
    Shader& shader = ShaderLibrary::getInstance().getShader(ShaderLibrary::ShaderType::TEXTURED);
    shader.setValue("projection", GameContext::getInstance().getProjectionMatrix());
    shader.setValue("transform", transform);
}

// Knows position, color, texture to draw
void Quad::draw() const {
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}
