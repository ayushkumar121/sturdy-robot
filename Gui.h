// Created by ari on 2/23/26.

#pragma once

#include <GLFW/glfw3.h>

#include "Font.h"
#include "Basic.h"
#include "Texture.h"
#include "Renderer.h"
#include "TextRenderer.h"

constexpr float DEFAULT_MARGIN = 20.0f;

class Gui {
public:
	Gui();
	~Gui();

	void begin(std::string_view label, Basic::Vec4 rect);
	void end();

	Basic::Vec2 getCursor();
	float getMargin() const;
	void setMargin(float margin);
	void moveCursor(float x, float y);
	void text(std::string_view text, Basic::Color color);
	bool button(std::string_view label);
	void image(Texture* texture, Basic::Vec2 size);
	bool imageButton(Texture* texture, Basic::Vec2 size);
	void rect(Basic::Color color, Basic::Vec2 size);
private:
	GLFWwindow *glfwWindow;
	const Font* font;

	Renderer renderer{};
	TextRenderer textRenderer{};
	Basic::Vec2 frameSize{};
	std::string_view label;
	Basic::Vec2 mouse{};
	Basic::Vec4 layout{};
	Basic::Vec2 cursor{};
	float margin{};
	bool mouseDown{};

	int getId() const;
	Basic::Vec2 transform(Basic::Vec2 point) const;
};
