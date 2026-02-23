#include <iostream>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Basic.h"
#include "Quad.h"
#include "Renderer.h"
#include "Texture.h"
#include "TextureLibrary.h"
#include "Gui.h"

int frameWidth;
int frameHeight;

std::vector<Quad> quads;

void viewportInit(GLFWwindow *window);
void frameBufferResizeCallback(GLFWwindow *window, int width, int height);
void eventHandler(GLFWwindow *window);
void errorCallback(int error, const char *description);
void debugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar * message,
                          const void * user_param);
void renderFrame(GLFWwindow *window);

int main() {
    glfwSetErrorCallback(errorCallback);
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow *window = glfwCreateWindow(1920, 1080, "Game", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGL(glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    if (glDebugMessageCallback) {
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(debugMessageCallback, nullptr);
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    std::cout << "GL_VERSION: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;

    viewportInit(window);
    glfwSetFramebufferSizeCallback(window, frameBufferResizeCallback);
    glfwSetWindowRefreshCallback(window, renderFrame);

    // Loading textures
    TextureLibrary& textureLib = TextureLibrary::getInstance();
    Texture& tex1 = textureLib.getTexture("assets/wall.jpg");
    Texture& tex2 = textureLib.getTexture("assets/awesomeface.png");
    Texture& tex3 = textureLib.getTexture("assets/sheet.png");

    quads.emplace_back(0.0f, 0.0f, 400.0f, 400.0f, Basic::Vec4{1.0f, 1.0f, 1.0f, 1.0f}, &tex1);
    quads.emplace_back(300.0f, 0.0f, 100.0f, 100.0f, Basic::Vec4{1.0f, 1.0f, 1.0f, 1.0f}, &tex2);
    quads.emplace_back(50.0f, 50.0f, 60.0f, 60.0f, Basic::hexColor(0xFF00FF00), nullptr);
    quads.emplace_back(300.0f, 400.0f, 500.0f, 400.0f, Basic::hexColor(0xFFFF0000), &tex3);

    std::cout << "Starting main loop" << std::endl;

    while (!glfwWindowShouldClose(window)) {
        eventHandler(window);
        renderFrame(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void viewportInit(GLFWwindow *window) {
    glfwGetFramebufferSize(window, &frameWidth, &frameHeight);
    glViewport(0, 0, frameWidth, frameHeight);
}

void frameBufferResizeCallback(GLFWwindow *window, int width, int height) {
    frameWidth = width;
    frameHeight = height;

    glViewport(0, 0, width, height);
}

void eventHandler(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void errorCallback(int error, const char *description) {
    std::cerr << "GLFW Error: " << description << std::endl;
}

void debugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *user_param) {
    std::cout <<"GL Message: " << message << std::endl;
}

void renderFrame(GLFWwindow *window) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Normal Rendering
    Renderer renderer;
    renderer.begin(Basic::Mat4::projection(frameWidth, frameHeight));

    float widthf = (float)frameWidth;
    float heightf = (float)frameHeight;

    Texture& tex4 = TextureLibrary::getInstance().getTexture("assets/office.jpg");
    Quad background(0.0f, 0.0f, widthf, heightf, Basic::hexColor(0xFFFFFFFF), &tex4);
    renderer.submit(background);

    for (auto &quad : quads) {
        renderer.submit(quad);
    }
    renderer.end();

    // GUI Layer
    Gui gui;
    gui.begin(window);
    if (gui.button({widthf/2.0f, heightf/2.0f, 300.0f, 100.0f})) {
        static int times = 0;
        std::cout << "Clicked " << times++ << std::endl;
    }
    gui.end();

    glfwSwapBuffers(window);
}