// Created by ari on 2/23/26.
#include "Gui.h"

#include <algorithm>

#include "FontLibrary.h"

const float buttonPadding = 20.0f;
static float scrollDelta = 0.0f;

static bool insideRect(Basic::Vec2 point, Basic::Vec4 rect) {
    return (point.x >= rect.x && point.x <= (rect.x + rect.z) &&
            point.y >= rect.y && point.y <= (rect.y + rect.w));
}

static void scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
    scrollDelta += (float) yoffset;
}

Gui::Gui() {
    this->window = glfwGetCurrentContext();
    this->font = &FontLibrary::getInstance().getFont(FontLibrary::FontType::PLAYFAIR);
    glfwSetScrollCallback(window, scrollCallback);
}

void Gui::begin() {
    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);

    int windowWidth, windowHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);

    glfwGetFramebufferSize(window, &frameSize.x, &frameSize.y);

    mouse.x = (float) mouseX * ((float) frameSize.x / (float) windowWidth);
    mouse.y = (float) mouseY * ((float) frameSize.y / (float) windowHeight);

    Basic::Vec4 rect = {0.0f, 0.0f, (float) frameSize.x, (float) frameSize.y};

    renderer.begin(rect);
    textRenderer.begin(font, rect);
}

void Gui::end() {
    renderer.end();
    textRenderer.end();
    scrollDelta = 0;
}

void Gui::text(std::string_view text, Basic::Vec4 rect, Basic::Color color) {
    size_t start = 0;
    size_t end = 0;

    float xPos = rect.x;
    float yPos = rect.y + font->getSize() + offsetY;

    for (const auto c: text) {
        if (c != ' ') {
            end++;
        } else {
            std::string_view word = text.substr(start, end - start + 1);
            Basic::Vec2 textSize = font->measureText(word);

            if (xPos + textSize.x > rect.x + rect.z) {
                xPos = rect.x;
                yPos += font->getSize() * font->getLineSpacing();
            }

            textRenderer.submit({word, {xPos, yPos}, color});
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
            yPos += font->getSize() * font->getLineSpacing();
        }

        textRenderer.submit({word, {xPos, yPos}, color});
    }
}

void Gui::scrollBegin(int x, int y, int width, int height) {
    glEnable(GL_SCISSOR_TEST);
    glScissor(x, frameSize.y - (y + height), width, height);

    Basic::Vec4 rect = {(float)x, (float)y, (float)width, (float)height};

    if (insideRect(mouse, rect)) {
        offsetY += scrollDelta * 50.0f;
    }

    Renderer::Quad panel{rect, Basic::hexColor(0x55FFFFFF), nullptr};
    renderer.submit(panel);
}

void Gui::scrollEnd() {
    renderer.end();
    renderer.clear();
    textRenderer.end();
    textRenderer.clear();
    scrollDelta = 0;
    glDisable(GL_SCISSOR_TEST);
}

bool Gui::button(std::string_view label, Basic::Vec2 pos) {
    int windowWidth, windowHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);

    int frameWidth, frameHeight;
    glfwGetFramebufferSize(window, &frameWidth, &frameHeight);

    Basic::Vec2 textSize = font->measureText(label);
    Basic::Vec4 rect = {pos.x, pos.y, textSize.x + 2 * buttonPadding, textSize.y + 2 * buttonPadding};

    static int prevState = GLFW_RELEASE;
    int newState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    bool hovered = insideRect({mouse.x, mouse.y}, rect);
    bool mouseDown = newState == GLFW_PRESS && prevState == GLFW_RELEASE;

    Basic::Vec4 color = hovered
                            ? (mouseDown ? Basic::hexColor(0xFFFF0000) : Basic::hexColor(0xFF00FF00))
                            : Basic::hexColor(0xFFFFFFFF);
    prevState = newState;

    Renderer::Quad quad(rect, color, nullptr);
    renderer.submit(quad);

    float textX = pos.x + buttonPadding;
    float textY = pos.y + (rect.w + textSize.y) / 2.0f;
    textRenderer.submit({label, {textX, textY}, Basic::hexColor(0xFF000000)});

    return hovered && mouseDown;
}
