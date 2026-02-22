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

int windowWidth = 640;
int windowHeight = 480;

void viewportInit(GLFWwindow *window);
void windowResizeCallback(GLFWwindow *window, int width, int height);
void eventHandler(GLFWwindow *window);
void errorCallback(int error, const char *description);
void debugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar * message,
                          const void * user_param);

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

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(debugMessageCallback, NULL);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    std::cout << "GL_VERSION: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;

    viewportInit(window);
    glfwSetFramebufferSizeCallback(window, windowResizeCallback);

    stbi_set_flip_vertically_on_load(true);

    // Loading textures
    TextureLibrary& textureLib = TextureLibrary::getInstance();
    Texture& tex1 = textureLib.getTexture("assets/wall.jpg");
    Texture& tex2 = textureLib.getTexture("assets/awesomeface.png");

    std::vector<Quad> quads;
    quads.emplace_back(0.0f, 0.0f, 400.0f, 400.0f, Basic::Vec4{1.0f, 1.0f, 1.0f, 1.0f}, &tex1);
    quads.emplace_back(300.0f, 0.0f, 100.0f, 100.0f, Basic::Vec4{1.0f, 1.0f, 1.0f, 1.0f}, &tex2);
    quads.emplace_back(50.0f, 50.0f, 60.0f, 60.0f, Basic::Vec4{0.3f, 0.5f, 0.0f, 0.5f}, nullptr);

    std::cout << "Starting main loop" << std::endl;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        eventHandler(window);

        /* Render here */
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        Renderer renderer;
        renderer.begin(Basic::Mat4::projection(width, height));
        for (auto &quad : quads) {
            renderer.submit(quad);
        }
        renderer.end();

        glfwSwapBuffers(window);
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

void debugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *user_param) {
    std::cout <<"GL Message: " << message << std::endl;
}
