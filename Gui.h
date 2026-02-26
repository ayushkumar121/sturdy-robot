// Created by ari on 2/23/26.

#pragma once

#include <vector>
#include <GLFW/glfw3.h>
#include <string>

#include "Font.h"
#include "Basic.h"
#include "Renderer.h"
#include "TextRenderer.h"

class Gui {
public:
	struct Content {
		Basic::Vec4 rect{};
		Basic::Vec2 cursor{};
		float scrollY = 0.0f;
	};

	Gui();

	void begin(Basic::Vec4 rect);
	void end();

	// void scrollBegin(int x, int y, int width, int height);
	// void scrollEnd();
	void text(std::string_view text, Basic::Color color);
	bool button(std::string_view label);
private:
	Basic::Vec2 transform(Basic::Vec2 point);
	Basic::Vec4 transform(Basic::Vec4 rect);

	GLFWwindow *window;
	const Font* font;

	Renderer renderer{};
	TextRenderer textRenderer{};
	Basic::Vec2 frameSize{};
	Basic::Vec2 mouse{};
	std::vector<Content> contentStack{};

	float offsetY = 0.0f;
};
