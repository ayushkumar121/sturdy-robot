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
	struct ContentState {
		int id;
		Basic::Vec4 rect{}; // absolute world coordinates
		Basic::Vec2 cursor{};
		std::vector<Renderer::Quad> quads;
		std::vector<TextRenderer::Text> texts;
	};

	Gui();
	~Gui();

	void begin(Basic::Vec2 size);
	void end();

	void text(std::string_view text, Basic::Color color);
	bool button(std::string_view label);
private:
	int id = 0;
	GLFWwindow *window;
	const Font* font;

	Renderer renderer{};
	TextRenderer textRenderer{};
	Basic::Vec2 frameSize{};
	Basic::Vec2 mouse{};
	std::vector<ContentState> contentStack{};
	int currenStateIndex = -1;

	Basic::Vec2 transform(const Basic::Vec2 point);
	ContentState& getCurrentState();
};
