// Created by ari on 2/23/26.
#include "Gui.h"

#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <functional>

#include "FontLibrary.h"

// GUI Config
constexpr float PADDING = 20.0f;
constexpr float SCROLL_SPEED = 40.0f;
constexpr float MARGIN = 20.0f;

static float scrollDelta = 0.0f;
static std::unordered_map<int, float> scrollData;

static bool insideRect(Basic::Vec2 point, Basic::Vec4 rect) {
    return (point.x >= rect.x && point.x <= (rect.x + rect.z) &&
            point.y >= rect.y && point.y <= (rect.y + rect.w));
}

static void scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
    scrollDelta += (float) yoffset;
}

Gui::Gui() {
    this->glfwWindow = glfwGetCurrentContext();
    this->font = &FontLibrary::getInstance().getFont(FontLibrary::FontType::ROBOTO);
    glfwSetScrollCallback(glfwWindow, scrollCallback);

    double mouseX, mouseY;
    glfwGetCursorPos(glfwWindow, &mouseX, &mouseY);

    int windowWidth, windowHeight;
    glfwGetWindowSize(glfwWindow, &windowWidth, &windowHeight);

    int frameWidth, frameHeight;
    glfwGetFramebufferSize(glfwWindow, &frameWidth, &frameHeight);
    frameSize = {(float)frameWidth, (float)frameHeight};

    mouse.x = (float) mouseX * (frameSize.x / (float) windowWidth);
    mouse.y = (float) mouseY * (frameSize.y / (float) windowHeight);
}

Gui::~Gui() = default;

Basic::Vec2 Gui::transform(Basic::Vec2 point) const {
    return Basic::Vec2{rect.x + point.x, rect.y + point.y + scrollData[getId()]};
}

void Gui::begin(std::string_view label, Basic::Vec4 rect) {
    this->label = label;
    this->rect = {rect.x + MARGIN, rect.y + MARGIN, rect.z - 2*MARGIN, rect.w - 2*MARGIN};
    this->cursor = {MARGIN, MARGIN};

    renderer.begin(frameSize);
    textRenderer.begin(font, frameSize);

    renderer.submit(Renderer::Quad{rect, Basic::hexColor(0x22FFFFFF), nullptr});
}

void Gui::end() {
    if (insideRect(mouse, rect)) {
        int id = getId();
        float scroll = scrollData[id];
        scroll += scrollDelta * SCROLL_SPEED;

        float maxScroll = cursor.y - rect.w;
        if (maxScroll < 0.0f) maxScroll = 0.0f;

        scroll = std::clamp(scroll, -maxScroll, 0.0f);
        scrollData[id] = scroll;
        scrollDelta = 0.0f;
    }

    glEnable(GL_SCISSOR_TEST);
    glScissor((int)rect.x,(int)(frameSize.y - (rect.y + rect.w)),(int)rect.z,(int)rect.w);

    renderer.end();
    textRenderer.end();

    glDisable(GL_SCISSOR_TEST);
}

int Gui::getId() const {
    return (int)std::hash<std::string_view>{}(label);
}

void Gui::text(std::string_view text, Basic::Color color) {
    Basic::Vec2 pos = transform(cursor);

    size_t start = 0;
    size_t end = 0;

    float xPos = pos.x;
    float yPos = pos.y + font->getSize();

    for (const auto c: text) {
        if (c != ' ') {
            end++;
        } else {
            std::string_view word = text.substr(start, end - start + 1);
            Basic::Vec2 textSize = font->measureText(word);

            if (xPos + textSize.x > pos.x + rect.z - MARGIN) {
                xPos = pos.x;
                yPos += font->getSize() * font->getLineSpacing();
            }

            textRenderer.submit({word, {xPos, yPos}, color});
            xPos += textSize.x;
            start = ++end;
        }
    }

    if (start < text.size()) {
        std::string_view word = text.substr(start, end - start);
        Basic::Vec2 textSize = font->measureText(word);

        if (xPos + textSize.x > pos.x + rect.z - MARGIN) {
            xPos = pos.x;
            yPos += font->getSize() * font->getLineSpacing();
        }

        textRenderer.submit({word, {xPos, yPos}, color});
    }
    cursor.y += yPos - pos.y + 2*MARGIN;
}

bool Gui::button(std::string_view text) {
    Basic::Vec2 pos = transform(cursor);

    Basic::Vec2 textSize = font->measureText(text);
    Basic::Vec4 rect = {pos.x, pos.y, textSize.x + 2 * PADDING, textSize.y + 2 * PADDING};

    static int prevState = GLFW_RELEASE;
    int newState = glfwGetMouseButton(glfwWindow, GLFW_MOUSE_BUTTON_LEFT);
    bool hovered = insideRect(mouse, rect);
    bool mouseDown = newState == GLFW_PRESS && prevState == GLFW_RELEASE;
    prevState = newState;

    Basic::Vec4 color = hovered? 
        (mouseDown ? Basic::hexColor(0xFFFF0000) : Basic::hexColor(0xFF00FF00))
        : Basic::hexColor(0xFFFFFFFF);
    renderer.submit({rect, color, nullptr});

    float textX = pos.x + PADDING;
    float textY = pos.y + (rect.w + textSize.y) / 2.0f;
    textRenderer.submit({text, {textX, textY}, Basic::hexColor(0xFF000000)});
    cursor.y += rect.w + 2*MARGIN;

    return hovered && mouseDown;
}
