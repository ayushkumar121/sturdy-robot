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

	bool button(std::string label, Basic::Vec2 pos);
private:
	GLFWwindow *window = nullptr;
	const Font* font = nullptr;
	Renderer renderer;
	TextRenderer textRenderer;
};
