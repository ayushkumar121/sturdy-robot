#include <string_view>
#include <iostream>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <filesystem>
#ifdef _WIN32
    #include <windows.h>
#elif __APPLE__
    #include <mach-o/dyld.h>
    #include <vector>
#else
    #include <unistd.h>
    #include <limits.h>
#endif

#include "Game.h"

void frameBufferResizeCallback(GLFWwindow *window, int width, int height);
void errorCallback(int error, const char *description);
void debugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar * message,
                          const void * user_param);
void updateFrame(GLFWwindow *window);
void changeWorkingDir(std::string_view exe_name);

static Game* game;

int main(int argc, char** argv) {
    changeWorkingDir(argv[0]);

    glfwSetErrorCallback(errorCallback);
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_FALSE);

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow *window = glfwCreateWindow(1920, 1080, "Game", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGL(glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    std::cerr << "GL_VERSION: " << glGetString(GL_VERSION) << std::endl;
    std::cerr << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cerr << "Vendor: " << glGetString(GL_VENDOR) << std::endl;

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
    std::cerr <<"GL Message: " << message << std::endl;
}

void updateFrame(GLFWwindow *window) {
    game->update(window);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    game->render(window);
    glfwSwapBuffers(window);
}

std::filesystem::path getExecutablePath() {
#ifdef _WIN32
    char buf[MAX_PATH];
    GetModuleFileNameA(nullptr, buf, MAX_PATH);
    return std::filesystem::canonical(buf);
#elif __APPLE__
    uint32_t size = 0;
    _NSGetExecutablePath(nullptr, &size);
    std::vector<char> buf(size);
    _NSGetExecutablePath(buf.data(), &size);
    return std::filesystem::canonical(buf.data());
#else
    char buf[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", buf, PATH_MAX);
    return std::filesystem::canonical(std::string(buf, count));
#endif
}

void changeWorkingDir(std::string_view exe_name) {
    try {
        std::filesystem::path exePath = getExecutablePath();
        std::filesystem::path targetDir;
        std::string exeStr = exePath.string();
        if (exeStr.find(".app") != std::string::npos) {
            targetDir = exePath.parent_path() / "Resources";
        } else {
            targetDir = exePath.parent_path();
        }
        std::cerr << "Changing directory to" << targetDir << std::endl;
        std::filesystem::current_path(targetDir);
    } catch (std::filesystem::filesystem_error& e) {
        std::cerr << "Error changing path: " << e.what() << "\n";
    }
}