// Created by ari on 2/23/26.

#pragma once

#include <GLFW/glfw3.h>
#include <string>

#include "Font.h"
#include "Basic.h"
#include "Renderer.h"
#include "TextRenderer.h"

class Gui {
public:
	void begin(GLFWwindow *window);
	void end();

	void text(std::string_view text, Basic::Vec4 rect, Basic::Color color);
	bool button(std::string_view label, Basic::Vec2 pos);
private:
	void submitGlyphs(std::string_view text, Basic::Vec2 pos, Basic::Color color);

	GLFWwindow *window = nullptr;
	const Font* font = nullptr;
	Renderer renderer;
	TextRenderer textRenderer;
};
