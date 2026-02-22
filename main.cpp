#include <iostream>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Basic.h"
#include "Shader.h"
#include "ShaderLibrary.h"
#include "Texture.h"

int windowWidth = 640;
int windowHeight = 480;

void viewportInit(GLFWwindow *window);
void windowResizeCallback(GLFWwindow *window, int width, int height);
void eventHandler(GLFWwindow *window);
void errorCallback(int error, const char *description);

int main() {
    glfwSetErrorCallback(errorCallback);
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow *window = glfwCreateWindow(windowWidth, windowHeight, "Game", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (!gladLoadGL(glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    std::cout << "GL_VERSION: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;

    viewportInit(window);
    glfwSetFramebufferSizeCallback(window, windowResizeCallback);

    constexpr float vertices[] = {
        // positions       // texture coords
        0.5f,  0.5f, 0.0f,  1.0f, 1.0f,   // top right
        0.5f, -0.5f, 0.0f,  1.0f, 0.0f,   // bottom right
       -0.5f, -0.5f, 0.0f,  0.0f, 0.0f,   // bottom left
       -0.5f,  0.5f, 0.0f,  0.0f, 1.0f    // top left
    };

    constexpr unsigned int indices[] = {
        // note that we start from 0!
        0, 1, 3, // first triangle
        1, 2, 3 // second triangle
    };

    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    unsigned int ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    stbi_set_flip_vertically_on_load(true);

    Texture tex1("assets/wall.jpg");
    Texture tex2("assets/awesomeface.png");

    auto transform = Basic::Mat4::identity();
    auto view = Basic::Mat4::identity();

    ShaderLibrary shaderLibrary;

    const Shader& shader = shaderLibrary.getShader(ShaderLibrary::ShaderType::TEXTURED);
    shader.bind();
    shader.setValue("tex1", 0);
    shader.setValue("tex2", 1);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        eventHandler(window);

        /* Render here */
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        tex1.bind(0);
        tex2.bind(1);

        shader.bind();
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        auto projection = Basic::Mat4::projection(width, height);

        Basic::Mat4 scaleMat = Basic::Mat4::scale(200.0f, 200.0f, 1.0f);
        Basic::Mat4 translationMat = Basic::Mat4::translate(width/2, height/2, 0.0f);
        shader.setValue("view", view);
        shader.setValue("projection", projection);
        shader.setValue("transform", transform * translationMat * scaleMat);

        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void viewportInit(GLFWwindow *window) {
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
}

void windowResizeCallback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void eventHandler(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void errorCallback(int error, const char *description) {
    std::cerr << "GLFW Error: " << description << std::endl;
}