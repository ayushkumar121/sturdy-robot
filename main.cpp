#include <iostream>
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "Basic.h"
#include "Renderer.h"
#include "Texture.h"
#include "TextureLibrary.h"
#include "Font.h"
#include "FontLibrary.h"
#include "Gui.h"
#include "TextRenderer.h"

void frameBufferResizeCallback(GLFWwindow *window, int width, int height);
void eventHandler(GLFWwindow *window);
void errorCallback(int error, const char *description);
void debugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar * message,
                          const void * user_param);
void renderFrame(GLFWwindow *window);

int main() {
    // TODO: Abtract away window creation
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

    Texture& tex4 = TextureLibrary::getInstance().getTexture("assets/officedark.jpg");
    Renderer::Quad background{screenRect, Basic::hexColor(0xFFFFFFFF), &tex4};
    renderer.submit(background);

    Texture& tex1 = TextureLibrary::getInstance().getTexture("assets/VN_Visiter_C.png");
    float characterHeight = (float)frameHeight;
    float characterWidth = (characterHeight/tex1.getHeight()) * tex1.getWidth();
    Renderer::Quad character{{0.0f, 0.0f, characterWidth, characterHeight}, Basic::hexColor(0xFFFFFFFF), &tex1};
    renderer.submit(character);

    Renderer::Quad panel{{0.0f, 3.0f*(float)frameHeight/4.0f, (float)frameWidth, (float)frameHeight/4.0f}, Basic::hexColor(0x55FFFFFF), nullptr};
    renderer.submit(panel);

    renderer.end();

    const Font& font = FontLibrary::getInstance().getFont(FontLibrary::FontType::PLAYFAIR);

    TextRenderer textRenderer;
    textRenderer.begin(&font, screenRect);
    std::string str = "In a hole in the ground there lived a hobbit.\n"
    "Not a nasty, dirty, wet hole, filled with the ends of worms and an oozy smell,\n"
    "nor yet a dry, bare, sandy hole with nothing in it to sit down on or to eat:\n"
    "it was a hobbit- hole, and that means comfort.";

    textRenderer.submit({str, {100.0f, 100.0f + 3.0f*(float)frameHeight/4.0f}, Basic::hexColor(0xFF000000)});
    textRenderer.end();

    // GUI Layer
    Gui gui;
    gui.begin(window);
    if (gui.button("Next", {3.0f*(float)frameWidth/4.0f, frameHeight - 100.0f})) {
        static int times = 0;
        std::cout << "Clicked " << times++ << std::endl;
    }
    gui.end();

    glfwSwapBuffers(window);
}