#include <iostream>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <format>

#include "Basic.h"
#include "Renderer.h"
#include "Texture.h"
#include "TextureLibrary.h"
#include "Font.h"
#include "Gui.h"
#include "TextRenderer.h"

std::vector<Renderer::Quad> quads;
Font* font;
Font* font2;

void frameBufferResizeCallback(GLFWwindow *window, int width, int height);
void eventHandler(GLFWwindow *window);
void errorCallback(int error, const char *description);
void debugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar * message,
                          const void * user_param);
void renderFrame(GLFWwindow *window);

int main() {
    // Abtract away window creation
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

    std::cout << "GL_VERSION: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;

    if (glDebugMessageCallback) {
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(debugMessageCallback, nullptr);
    }

    int frameWidth, frameHeight;
    glfwGetFramebufferSize(window, &frameWidth, &frameHeight);
    glViewport(0, 0, frameWidth, frameHeight);

    glfwSetFramebufferSizeCallback(window, frameBufferResizeCallback);
    glfwSetWindowRefreshCallback(window, renderFrame);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Loading textures
    TextureLibrary& textureLib = TextureLibrary::getInstance();
    Texture& tex1 = textureLib.getTexture("assets/wall.jpg");
    Texture& tex2 = textureLib.getTexture("assets/awesomeface.png");

    quads.emplace_back(Basic::Vec4{0.0f, 0.0f, 400.0f, 400.0f}, Basic::hexColor(0xFFFFFFFF), &tex1);
    quads.emplace_back(Basic::Vec4{300.0f, 0.0f, 100.0f, 100.0f}, Basic::hexColor(0xFFFFFFFF), &tex2);
    quads.emplace_back(Basic::Vec4{50.0f, 50.0f, 60.0f, 60.0f}, Basic::hexColor(0xFF00FF00), nullptr);

    font = new Font("assets/Playfair.ttf", 48);
    font2 = new Font("assets/SourceCodePro.ttf", 48);
    std::cout << "Starting main loop" << std::endl;

    while (!glfwWindowShouldClose(window)) {
        eventHandler(window);
        renderFrame(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void frameBufferResizeCallback(GLFWwindow *window, int width, int height) {
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

    // Basic::getScreenRect
    int frameWidth, frameHeight;
    glfwGetFramebufferSize(window, &frameWidth, &frameHeight);

    Basic::Vec4 screenRect{0.0f, 0.0f, (float)frameWidth, (float)frameHeight};
 
    // Normal Rendering
    Renderer renderer;
    renderer.begin(screenRect);

    // Texture& tex4 = TextureLibrary::getInstance().getTexture("assets/office.jpg");
    // Renderer::Quad background{screenRect, Basic::hexColor(0xFFFFFFFF), &tex4};
    // renderer.submit(background);

    for (auto &quad : quads) {
        renderer.submit(quad);
    }
    renderer.end();

    TextRenderer textRenderer;
    textRenderer.begin(font, screenRect);
    std::string str = "In a hole in the ground there lived a hobbit.\n"
    "Not a nasty, dirty, wet hole, filled with the ends of worms and an oozy smell,\n"
    "nor yet a dry, bare, sandy hole with nothing in it to sit down on or to eat:\n"
    "it was a hobbit- hole, and that means comfort.";

    // textRenderer.submit({"Hello World", {(float)frameWidth/2, (float)frameHeight/2}, Basic::hexColor(0xFF00FF00)});
    textRenderer.submit({str, {600.0f, 600.0f}, Basic::hexColor(0xFFAAFFBB)});
    textRenderer.end();

    // GUI Layer
    Gui gui;
    gui.begin(window, font2);
    if (gui.button("Hello Worldg", {(float)frameWidth/2.0f, (float)frameHeight/2.0f})) {
        static int times = 0;
        std::cout << "Clicked " << times++ << std::endl;
    }
    gui.end();

    glfwSwapBuffers(window);
}