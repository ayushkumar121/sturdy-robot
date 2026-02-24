// Created by ari on 2/23/26.
#include "Gui.h"

#include <cassert>

const

static inline bool insideRect(Basic::Vec2 point, Basic::Vec4 rect) {
    return (point.x >= rect.x && point.x <= (rect.x+rect.z) && 
            point.y >= rect.y && point.y <= (rect.y+rect.w));
}

void Gui::begin(GLFWwindow *window, Font *font) {
	this->window = window;
    this->font = font;

    // GUI starts a full screen rendering
    int frameWidth, frameHeight;
	glfwGetFramebufferSize(window, &frameWidth, &frameHeight);

    Basic::Vec4 rect = {0.0f, 0.0f, (float)frameWidth, (float)frameHeight};
    renderer.begin(rect);
    textRenderer.begin(font, rect);
}

void Gui::end() {
    renderer.end();
    textRenderer.end();
}

bool Gui::button(std::string label, Basic::Vec2 pos) {
	assert(window != nullptr);

 	double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    int windowWidth, windowHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);

    int frameWidth, frameHeight;
	glfwGetFramebufferSize(window, &frameWidth, &frameHeight);

    float mouseX = (float)xpos * ((float)frameWidth / (float)windowWidth);
    float mouseY = (float)ypos * ((float)frameHeight / (float)windowHeight);

    Basic::Vec2 textSize = font->measureText(label);
    float pad = 20.0f;
    Basic::Vec4 rect = {pos.x, pos.y, textSize.x + 2 * pad, textSize.y + 2 * pad};

    bool hovered = insideRect({mouseX, mouseY}, rect);
	bool mouseDown = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
    Basic::Vec4 color = hovered? (mouseDown? Basic::hexColor(0xFFFF0000): Basic::hexColor(0xFF00FF00)): Basic::hexColor(0xFFFFFFFF);

    Renderer::Quad quad(rect, color, nullptr);
    renderer.submit(quad);
    
    float textX = pos.x + pad;
    float textY = pos.y + (rect.w + textSize.y) / 2.0f;
    textRenderer.submit({label, {textX, textY}, Basic::hexColor(0xFF000000)});

    return hovered && mouseDown;
}
