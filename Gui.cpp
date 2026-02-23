#include "Gui.h"

#include <cassert>

#include "Quad.h"

static inline bool insideRect(Basic::Vec2 point, Basic::Vec4 rect) {
    return (point.x >= rect.x && point.x <= (rect.x+rect.z) && 
            point.y >= rect.y && point.y <= (rect.y+rect.w));
}

void Gui::begin(GLFWwindow *window) {
	this->window = window;

    int frameWidth, frameHeight;
	glfwGetFramebufferSize(window, &frameWidth, &frameHeight);

    renderer.begin(Basic::Mat4::projection(frameWidth, frameHeight));
}

void Gui::end() {
    renderer.end();
}

bool Gui::button(Basic::Vec4 rect) {
	assert(window != nullptr);

 	double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    int windowWidth, windowHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);

    int frameWidth, frameHeight;
	glfwGetFramebufferSize(window, &frameWidth, &frameHeight);

    float mouseX = (float)xpos * ((float)frameWidth / (float)windowWidth);
    float mouseY = (float)ypos * ((float)frameHeight / (float)windowHeight);

    bool hovered = insideRect({mouseX, mouseY}, rect);
	bool mouseDown = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
    Basic::Vec4 color = hovered? (mouseDown? Basic::hexColor(0xFFFF0000): Basic::hexColor(0xFF00FF00)): Basic::hexColor(0xFFFFFFFF);

    Quad quad(rect.x, rect.y, rect.z, rect.w, color, nullptr);
    renderer.submit(quad);

    return hovered && mouseDown;
}
