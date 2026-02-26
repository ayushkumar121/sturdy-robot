// Created by ari on 2/23/26.
#include "Gui.h"

#include <iostream>
#include <algorithm>

#include "FontLibrary.h"

// GUI Config
const float PADDING = 20.0f;
const float SCROLL_SPEED = 40.0f;
const float MARGIN = 15.0f;

static float scrollDelta = 0.0f;

static bool insideRect(Basic::Vec2 point, Basic::Vec4 rect) {
    return (point.x >= rect.x && point.x <= (rect.x + rect.z) &&
            point.y >= rect.y && point.y <= (rect.y + rect.w));
}

// If words wrap then what would be the height
static float measureTextHeight(const Font* font, std::string_view text, float x, float width)  {
    size_t start = 0;
    size_t end = 0;

    float xPos = x;
    float yPos = font->getSize();

    for (const auto c: text) {
        if (c != ' ') {
            end++;
        } else {
            std::string_view word = text.substr(start, end - start + 1);
            Basic::Vec2 textSize = font->measureText(word);

            if (xPos + textSize.x > x + width) {
                xPos = x;
                yPos += font->getSize() * font->getLineSpacing();
            }
            xPos += textSize.x;
            start = ++end;
        }
    }

    if (start < text.size()) {
        std::string_view word = text.substr(start, end - start);
        Basic::Vec2 textSize = font->measureText(word);

        if (xPos + textSize.x > x + width) {
            xPos = x;
            yPos += font->getSize() * font->getLineSpacing();
        }
    }

    return yPos;
}

static void scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
    scrollDelta += (float) yoffset;
}

Gui::Gui() {
    this->window = glfwGetCurrentContext();
    this->font = &FontLibrary::getInstance().getFont(FontLibrary::FontType::ROBOTO);
    glfwSetScrollCallback(window, scrollCallback);

    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);

    int windowWidth, windowHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);

    int frameWidth, frameHeight;
    glfwGetFramebufferSize(window, &frameWidth, &frameHeight);
    frameSize = {(float)frameWidth, (float)frameHeight};

    mouse.x = (float) mouseX * (frameSize.x / (float) windowWidth);
    mouse.y = (float) mouseY * (frameSize.y / (float) windowHeight);
}

void Gui::begin(Basic::Vec4 rect) {
    glEnable(GL_SCISSOR_TEST);
    glScissor((int)rect.x, (int)(frameSize.y - (rect.y + rect.w)), (int)rect.z, (int)rect.w);

    Content content;
    content.rect = rect;
    content.cursor = {0.0f, 0.0f};

    renderer.begin(frameSize);
    textRenderer.begin(font, frameSize);

    contentStack.push_back(content);

    if (insideRect(mouse, rect)) {
        content.scrollY += scrollDelta * SCROLL_SPEED;
        // content.scrollY = offsetY = std::min(0.0f, offsetY);
    }
}

void Gui::end() {
    renderer.end();
    textRenderer.end();
    contentStack.pop_back();
    glDisable(GL_SCISSOR_TEST);
    scrollDelta = 0;
}

// void Gui::scrollBegin(int x, int y, int width, int height) {
//     glEnable(GL_SCISSOR_TEST);
//     glScissor(x, frameSize.y - (y + height), width, height);
//
//     // scrollRect = {(float)x, (float)y, (float)width, (float)height};
//     //
//     // if (insideRect(mouse, scrollRect)) {
//     //     offsetY += scrollDelta * SCROLL_SPEED;
//     //     offsetY = std::min(0.0f, offsetY);
//     // }
//     //
//     // Renderer::Quad scrollBackground{scrollRect, Basic::hexColor(0x55FFFFFF), nullptr};
//     // renderer.submit(scrollBackground);
// }
//
// void Gui::scrollEnd() {
//     renderer.end();
//     renderer.clear();
//     textRenderer.end();
//     textRenderer.clear();
//     scrollDelta = 0;
//     glDisable(GL_SCISSOR_TEST);
// }

void Gui::text(std::string_view text, Basic::Color color) {
    Content& content = contentStack.back();
    Basic::Vec2 pos = transform(content.cursor);
    // Kind of a hack
    // float textContentHeight = measureTextHeight(font, text, pos.x, content.contentSize.x);
    // offsetY = std::max(offsetY, -textContentHeight);

    size_t start = 0;
    size_t end = 0;

    float xPos = pos.x;
    float yPos = pos.y + font->getSize();// + offsetY

    for (const auto c: text) {
        if (c != ' ') {
            end++;
        } else {
            std::string_view word = text.substr(start, end - start + 1);
            Basic::Vec2 textSize = font->measureText(word);

            if (xPos + textSize.x > pos.x + content.rect.z - MARGIN) {
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

        if (xPos + textSize.x > pos.x + content.rect.z - MARGIN) {
            xPos = pos.x;
            yPos += font->getSize() * font->getLineSpacing();
        }

        textRenderer.submit({word, {xPos, yPos}, color});
    }

    // Renderer::Quad quad{{pos.x, pos.y, content.contentSize.x, yPos - pos.y}, Basic::hexColor(0xFFFFFFFF), nullptr};
    // renderer.submit(quad);

    content.cursor.y = yPos + 2*MARGIN;
}

bool Gui::button(std::string_view label) {
    Content& content = contentStack.back();
    Basic::Vec2 pos = transform(content.cursor);

    Basic::Vec2 textSize = font->measureText(label);
    Basic::Vec4 rect = {pos.x, pos.y, textSize.x + 2 * PADDING, textSize.y + 2 * PADDING};

    Basic::Color color = Basic::hexColor(0xFFFFFFFF);
    Renderer::Quad quad(rect, color, nullptr);
    renderer.submit(quad);

    float textX = pos.x + PADDING;
    float textY = pos.y + (rect.w + textSize.y) / 2.0f;
    textRenderer.submit({label, {textX, textY}, Basic::hexColor(0xFF000000)});

    content.cursor.y += rect.w + 2*MARGIN;

    return false;

    // int windowWidth, windowHeight;
    // glfwGetWindowSize(window, &windowWidth, &windowHeight);
    //
    // int frameWidth, frameHeight;
    // glfwGetFramebufferSize(window, &frameWidth, &frameHeight);
    //
    // Basic::Vec2 textSize = font->measureText(label);
    // Basic::Vec4 rect = {pos.x, pos.y, textSize.x + 2 * buttonPadding, textSize.y + 2 * buttonPadding};
    //
    // static int prevState = GLFW_RELEASE;
    // int newState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    // bool hovered = insideRect({mouse.x, mouse.y}, rect);
    // bool mouseDown = newState == GLFW_PRESS && prevState == GLFW_RELEASE;
    //
    // Basic::Vec4 color = hovered
    //                         ? (mouseDown ? Basic::hexColor(0xFFFF0000) : Basic::hexColor(0xFF00FF00))
    //                         : Basic::hexColor(0xFFFFFFFF);
    // prevState = newState;
    //
    // Renderer::Quad quad(rect, color, nullptr);
    // renderer.submit(quad);
    //
    // float textX = pos.x + buttonPadding;
    // float textY = pos.y + (rect.w + textSize.y) / 2.0f;
    // textRenderer.submit({label, {textX, textY}, Basic::hexColor(0xFF000000)});
    //
    // return hovered && mouseDown;
}

Basic::Vec2 Gui::transform(Basic::Vec2 point) {
    float x = point.x;
    float y = point.y;

    for (const auto &content: contentStack) {
        x += content.rect.x + MARGIN;
        y += content.rect.y + MARGIN + content.scrollY;
    }

    return {x, y};
}

Basic::Vec4 Gui::transform(Basic::Vec4 rect) {
    Basic::Vec2 pos = transform(Basic::Vec2{rect.x, rect.y});
    return {pos.x, pos.y, rect.z, rect.w};
}
