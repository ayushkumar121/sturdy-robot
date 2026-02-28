// Created by ari on 2/23/26.

#pragma once

#include <GLFW/glfw3.h>

#include "Font.h"
#include "Basic.h"
#include "Renderer.h"
#include "TextRenderer.h"

class Gui {
public:
	Gui();
	~Gui();

	void begin(std::string_view label, Basic::Vec4 rect);
	void end();

	void text(std::string_view text, Basic::Color color);
	bool button(std::string_view label);
	int getId() const;
private:
	GLFWwindow *glfwWindow;
	const Font* font;

	Renderer renderer{};
	TextRenderer textRenderer{};
	Basic::Vec2 frameSize{};
	std::string_view label;
	Basic::Vec2 mouse{};
	Basic::Vec4 rect{};
	Basic::Vec2 cursor{};

	Basic::Vec2 transform(Basic::Vec2 point) const;
};
