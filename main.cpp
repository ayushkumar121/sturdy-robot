#include <iostream>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Basic.h"
#include "GameContext.h"
#include "Quad.h"
#include "Shader.h"
#include "ShaderLibrary.h"
#include "Texture.h"

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

    std::cout << "GL_VERSION: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;

    viewportInit(window);
    glfwSetFramebufferSizeCallback(window, windowResizeCallback);

    Quad quad(0.0f, 0.0f, 300.0f, 400.0f, {0.0f, 0.0f, 0.0f, 0.0f});
    Quad quad2(300.0f, 400.0f, 300.0f, 400.0f, {0.0f, 0.0f, 0.0f, 0.0f});

    stbi_set_flip_vertically_on_load(true);

    Texture tex1("assets/wall.jpg");
    Texture tex2("assets/awesomeface.png");

    auto view = Basic::Mat4::identity();

    const Shader& shader = ShaderLibrary::getInstance().getShader(ShaderLibrary::ShaderType::TEXTURED);
    shader.bind();
    shader.setValue("tex1", 0);
    shader.setValue("tex2", 1);

    std::cout << "Starting main loop" << std::endl;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        eventHandler(window);

        /* Render here */
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        tex1.bind(0);
        tex2.bind(1);

        shader.bind();
        shader.setValue("view", view);

        quad.draw();
        quad2.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void viewportInit(GLFWwindow *window) {
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    GameContext::getInstance().setFrameWidth(width);
    GameContext::getInstance().setFrameHeight(height);

    glViewport(0, 0, width, height);
}

void windowResizeCallback(GLFWwindow *window, int width, int height) {
    GameContext::getInstance().setFrameWidth(width);
    GameContext::getInstance().setFrameHeight(height);

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
