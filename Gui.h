// Created by ari on 2/23/26.

#pragma once

#include <stack>
#include <GLFW/glfw3.h>
#include <string>

#include "Font.h"
#include "Basic.h"
#include "Renderer.h"
#include "TextRenderer.h"

class Gui {
public:
	Gui();

	void begin();
	void end();

	void scrollBegin(int x, int y, int width, int height);
	void scrollEnd();
	void text(std::string_view text, Basic::Vec4 rect, Basic::Color color);
	bool button(std::string_view label, Basic::Vec2 pos);
private:
	GLFWwindow *window;
	const Font* font;

	Renderer renderer{};
	TextRenderer textRenderer{};
	Basic::IVec2 frameSize{};
	Basic::Vec2 mouse{};

	float offsetY = 0.0f;
};
