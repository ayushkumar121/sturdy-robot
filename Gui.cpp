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

static float scrollDelta = 0.0f;
static std::unordered_map<int, float> scrollData;

static bool insideRect(Basic::Vec2 point, Basic::Vec4 rect) {
    return (point.x >= rect.x && point.x <= (rect.x + rect.z) &&
            point.y >= rect.y && point.y <= (rect.y + rect.w));
}

static void scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
    scrollDelta += (float) yoffset;
}

void Gui::update(GLFWwindow* window) {
    glfwSetScrollCallback(window, scrollCallback);

    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);

    int windowWidth, windowHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);

    int frameWidth, frameHeight;
    glfwGetFramebufferSize(window, &frameWidth, &frameHeight);
    frameSize = {(float)frameWidth, (float)frameHeight};

    static int prevState = GLFW_RELEASE;
    int newState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    mouseDown = newState == GLFW_PRESS && prevState == GLFW_RELEASE;
    prevState = newState;

    mouse.x = (float) mouseX * (frameSize.x / (float) windowWidth);
    mouse.y = (float) mouseY * (frameSize.y / (float) windowHeight);
}

Basic::Vec2 Gui::transform(Basic::Vec2 point) const {
    return Basic::Vec2{layout.x + point.x, layout.y + point.y + scrollData[getId()]};
}

void Gui::begin(std::string_view label, Basic::Vec4 rect, bool scrollable) {
    this->label = label;
    this->layout = rect;
    this->cursor = {0.0f, 0.0f};
    this->margin = DEFAULT_MARGIN;
    this->scrollable = scrollable;

    renderer.begin(frameSize);
    textRenderer.begin(frameSize);
}

void Gui::end() {
    if (scrollable && insideRect(mouse, layout)) {
        int id = getId();
        float scroll = scrollData[id];
        scroll += scrollDelta * SCROLL_SPEED;

        float maxScroll = cursor.y - layout.w;
        if (maxScroll < 0.0f) maxScroll = 0.0f;

        scroll = std::clamp(scroll, -maxScroll, 0.0f);
        scrollData[id] = scroll;
        scrollDelta = 0.0f;
    }

    glEnable(GL_SCISSOR_TEST);
    glScissor((int)layout.x,(int)(frameSize.y - (layout.y + layout.w)),(int)layout.z,(int)layout.w);

    renderer.end();
    textRenderer.end();

    glDisable(GL_SCISSOR_TEST);
}

Basic::Vec2 Gui::getCursor() {
    return cursor;
}

void Gui::setCursor(Basic::Vec2 newCursor) {
    cursor = newCursor;
}

float Gui::getMargin() const {
    return margin;
}

void Gui::setMargin(float margin) {
    this->margin = margin;
}

int Gui::getId() const {
    return (int)std::hash<std::string_view>{}(label);
}

void Gui::text(std::string_view text, Basic::Color color, const Font* font) {
    if (font == nullptr) {
        font = &FontLibrary::getInstance().getFont(FontLibrary::FontType::ROBOTO);
    }

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

            if (xPos + textSize.x > pos.x + layout.z - margin) {
                xPos = pos.x;
                yPos += font->getSize() * font->getLineSpacing();
            }

            textRenderer.submit({std::string(word), {xPos, yPos}, color, font});
            xPos += textSize.x;
            start = ++end;
        }
    }

    if (start < text.size()) {
        std::string_view word = text.substr(start, end - start);
        Basic::Vec2 textSize = font->measureText(word);

        if (xPos + textSize.x > pos.x + layout.z - margin) {
            xPos = pos.x;
            yPos += font->getSize() * font->getLineSpacing();
        }

        textRenderer.submit({std::string(word), {xPos, yPos}, color, font});
    }
    cursor.y += yPos - pos.y + 2*margin;
}

bool Gui::button(std::string_view text) {
    const Font* font = &FontLibrary::getInstance().getFont(FontLibrary::FontType::ROBOTO);

    Basic::Vec2 pos = transform(cursor);
    Basic::Vec2 textSize = font->measureText(text);
    Basic::Vec4 rect = {pos.x, pos.y, textSize.x + 2 * PADDING, textSize.y + 2 * PADDING};

    bool hovered = insideRect(mouse, rect);
    Basic::Vec4 color = hovered?
        (mouseDown ? Basic::hexColor(0xFFFF0000) : Basic::hexColor(0xFF00FF00))
        : Basic::hexColor(0xFFFFFFFF);
    renderer.submit({rect, color, nullptr});

    float textX = pos.x + PADDING;
    float textY = pos.y + (rect.w + textSize.y) / 2.0f;
    textRenderer.submit({std::string(text), {textX, textY}, Basic::hexColor(0xFF000000), font});
    cursor.y += rect.w + margin;

    return hovered && mouseDown;
}

void Gui::image(Texture* texture, Basic::Vec2 size) {
    Basic::Vec2 pos = transform(cursor);
    Basic::Vec4 rect = {pos.x, pos.y, size.x, size.y};
    renderer.submit({rect, Basic::hexColor(0xFFFFFFFF), texture});
    cursor.y += rect.w + margin;
}

bool Gui::imageButton(Texture* texture, Basic::Vec2 size) {
    Basic::Vec2 pos = transform(cursor);
    Basic::Vec4 rect = {pos.x, pos.y, size.x, size.y};

    bool hovered = insideRect(mouse, rect);
    Basic::Vec4 color = hovered? Basic::hexColor(0x55FFFFFF):Basic::hexColor(0xFFFFFFFF);
    renderer.submit({rect, color, texture});
    cursor.y += rect.w + margin;
    return hovered && mouseDown;
}

void Gui::rect(Basic::Color color, Basic::Vec2 size) {
    Basic::Vec2 pos = transform(cursor);
    Basic::Vec4 rect = {pos.x, pos.y, size.x, size.y};
    renderer.submit({rect, color, nullptr});
    cursor.y += rect.w + margin;
}