// Created by ari on 2/23/26.
#include "Gui.h"

#include "FontLibrary.h"

constexpr float lineSpacing = 1.25f;

static bool insideRect(Basic::Vec2 point, Basic::Vec4 rect) {
    return (point.x >= rect.x && point.x <= (rect.x+rect.z) && 
            point.y >= rect.y && point.y <= (rect.y+rect.w));
}

void Gui::begin(GLFWwindow *window) {
	this->window = window;
    this->font = &FontLibrary::getInstance().getFont(FontLibrary::FontType::PLAYFAIR);

    // GUI starts a full screen rendering
    int frameWidth, frameHeight;
	glfwGetFramebufferSize(window, &frameWidth, &frameHeight);

    Basic::Vec4 rect = {0.0f, 0.0f, (float)frameWidth, (float)frameHeight};
    renderer.begin(rect);
    textRenderer.begin(rect);
}

void Gui::end() {
    renderer.end();
    textRenderer.end();
}

void Gui::submitGlyphs(std::string_view text, Basic::Vec2 pos, Basic::Color color) {
	float offsetX = 0.0f;
	float offsetY = 0.0f;
	for (auto ch : text) {
		if (ch == '\n') {
			offsetX = 0.0f;
			offsetY += font->getSize() * lineSpacing;
			continue;
		}
		const Font::Face& face = font->getFace(ch);
		if (face.textureId == 0) {
			offsetX += face.advance >> 6;
			continue;
		}
		float glyphX = pos.x + offsetX + face.bearingX;
		float glyphY = pos.y + offsetY - face.bearingY;
		textRenderer.submit({face.textureId, {glyphX, glyphY}, {(float)face.width, (float)face.height}, color});
		offsetX += face.advance >> 6;
	}
}

void Gui::text(std::string_view text, Basic::Vec4 rect, Basic::Color color) {
	size_t start = 0;
	size_t end = 0;

	float xPos = rect.x;
	float yPos = rect.y + font->getSize();

	for (const auto c : text) {
		if (c != ' ') {
			end++;
		} else {
			std::string_view word = text.substr(start, end - start + 1);
			Basic::Vec2 textSize = font->measureText(word);

			if (xPos + textSize.x > rect.x + rect.z) {
				xPos = rect.x;
				yPos += font->getSize() * lineSpacing;
			}

			submitGlyphs(word, {xPos, yPos}, color);
			xPos += textSize.x;

			end++;
			start = end;
		}
	}

	if (start < text.size()) {
		std::string_view word = text.substr(start, end - start);
		Basic::Vec2 textSize = font->measureText(word);

		if (xPos + textSize.x > rect.x + rect.z) {
			xPos = rect.x;
			yPos += font->getSize() * lineSpacing;
		}

		submitGlyphs(word, {xPos, yPos}, color);
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
    submitGlyphs(label, {textX, textY}, Basic::hexColor(0xFF000000));

    return hovered && mouseDown;
}
