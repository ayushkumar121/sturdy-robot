// Created by ari on 2/23/26.
#include "Gui.h"

#include <iostream>

#include "FontLibrary.h"

static bool insideRect(Basic::Vec2 point, Basic::Vec4 rect) {
    return (point.x >= rect.x && point.x <= (rect.x+rect.z) && 
            point.y >= rect.y && point.y <= (rect.y+rect.w));
}

void Gui::begin(GLFWwindow *window) {
	this->window = window;
    this->font = &FontLibrary::getInstance().getFont(FontLibrary::FontType::PRESS_START);

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

void Gui::text(std::string_view text, Basic::Vec4 rect, Basic::Color color) {
	size_t start = 0;
	size_t end = 0;

	float xPos = rect.x;
	float yPos = rect.y;

	Renderer::Quad quad(rect, Basic::hexColor(0x5500FFFF), nullptr);
	renderer.submit(quad);

	for (const auto c : text) {
		if (c != ' ') {
			end++;
		} else {
			std::string_view word = text.substr(start, end - start + 1);
			Basic::Vec2 textSize = font->measureText(word);

			textRenderer.submit({word, {xPos, yPos}, color});

			if (xPos + textSize.x > rect.x + rect.z) {
				xPos = rect.x;
				yPos += textSize.y;
			} else {
				xPos += textSize.x;
			}
			end++;
			start = end;
		}
	}
}

bool Gui::button(std::string_view label, Basic::Vec2 pos) {
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

	static int prevState = GLFW_RELEASE;
	int newState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);

    bool hovered = insideRect({mouseX, mouseY}, rect);
	bool mouseDown = newState == GLFW_PRESS && prevState == GLFW_RELEASE;
    Basic::Vec4 color = hovered? (mouseDown? Basic::hexColor(0xFFFF0000): Basic::hexColor(0xFF00FF00)): Basic::hexColor(0xFFFFFFFF);

	prevState = newState;

    Renderer::Quad quad(rect, color, nullptr);
    renderer.submit(quad);
    
    float textX = pos.x + pad;
    float textY = pos.y + (rect.w + textSize.y) / 2.0f;
    textRenderer.submit({label, {textX, textY}, Basic::hexColor(0xFF000000)});

    return hovered && mouseDown;
}
