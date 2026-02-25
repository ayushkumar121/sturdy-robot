#include <iostream>
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "Game.h"

void frameBufferResizeCallback(GLFWwindow *window, int width, int height);
void errorCallback(int error, const char *description);
void debugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar * message,
                          const void * user_param);
void updateFrame(GLFWwindow *window);

static Game* game;

int main() {
    // TODO: Abtract away window creation
    glfwSetErrorCallback(errorCallback);
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_FALSE);

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow *window = glfwCreateWindow(1920, 1080, "Game", glfwGetPrimaryMonitor(), nullptr);
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
    glfwSetWindowRefreshCallback(window, updateFrame);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    game = new Game();
    glfwSetWindowUserPointer(window, game);

    while (!glfwWindowShouldClose(window)) {
        updateFrame(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void frameBufferResizeCallback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void errorCallback(int error, const char *description) {
    std::cerr << "GLFW Error: " << description << std::endl;
}

void debugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *user_param) {
    std::cout <<"GL Message: " << message << std::endl;
}

void updateFrame(GLFWwindow *window) {
    game->update(window);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    game->render(window);
    glfwSwapBuffers(window);
}