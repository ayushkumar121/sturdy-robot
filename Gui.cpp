// Created by ari on 2/23/26.
#include "Gui.h"

#include <iostream>
#include <algorithm>
#include <unordered_map>

#include "FontLibrary.h"

// GUI Config
const float PADDING = 20.0f;
const float SCROLL_SPEED = 40.0f;
const float MARGIN = 15.0f;

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

    // renderer.begin(frameSize);
    // textRenderer.begin(font, frameSize);
}

Gui::~Gui() {
    glEnable(GL_SCISSOR_TEST);
    // Drawing front to back
    while(!contentStack.empty()) {
        ContentState& state = contentStack.front();
        glScissor((int)state.rect.x, (int)(frameSize.y - (state.rect.y + state.rect.w)), (int)state.rect.z, (int)state.rect.w);
        
        renderer.begin(frameSize);
        for (auto quad : state.quads) {
            renderer.submit(quad);
        }
        renderer.end();

        textRenderer.begin(font, frameSize);
        for (auto text : state.texts) {
            textRenderer.submit(text);
        }
        textRenderer.end();
        contentStack.erase(contentStack.begin());
    }
    glDisable(GL_SCISSOR_TEST);
    scrollDelta = 0.0f;
}

void Gui::begin(Basic::Vec2 size) {
    Basic::Vec4 rect;
    if (currenStateIndex >= 0) {
        ContentState& parent = getCurrentState();

        float left   = parent.rect.x + parent.cursor.x;
        float top    = parent.rect.y + parent.cursor.y + scrollData[parent.id];
        float right  = std::min(parent.rect.x + parent.rect.z, left + size.x);
        float bottom = std::min(parent.rect.y + parent.rect.w, top + size.y);

        rect = {left, top, std::max(0.0f, right - left - MARGIN), std::max(0.0f, bottom - top - MARGIN)};
        parent.cursor.y += size.y + MARGIN;
    } else {
        rect = {MARGIN, MARGIN, size.x - MARGIN, size.y - MARGIN};
    }

    ContentState state;
    state.id = id++;
    state.cursor = {MARGIN, MARGIN};
    state.rect = rect;
    state.quads.push_back(Renderer::Quad{rect, Basic::hexColor(0x55FFFFFF), nullptr});
    contentStack.push_back(state);

    currenStateIndex++;
}

void Gui::end() {
    ContentState& state = getCurrentState();
    if (insideRect(mouse, state.rect)) {
        scrollData[state.id] += scrollDelta * SCROLL_SPEED;

        float maxScroll = state.cursor.y - state.rect.w;
        if (maxScroll < 0.0f) maxScroll = 0.0f;
        scrollData[state.id] = std::clamp(scrollData[state.id], -maxScroll, 0.0f);
        scrollDelta = 0;
    }

    currenStateIndex--;
}

void Gui::text(std::string_view text, Basic::Color color) {
    ContentState& state = getCurrentState();
    Basic::Vec2 pos = transform(state.cursor);

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

            if (xPos + textSize.x > pos.x + state.rect.z - MARGIN) {
                xPos = pos.x;
                yPos += font->getSize() * font->getLineSpacing();
            }

            state.texts.push_back(TextRenderer::Text{word, {xPos, yPos}, color});
            xPos += textSize.x;
            start = ++end;
        }
    }

    if (start < text.size()) {
        std::string_view word = text.substr(start, end - start);
        Basic::Vec2 textSize = font->measureText(word);

        if (xPos + textSize.x > pos.x + state.rect.z - MARGIN) {
            xPos = pos.x;
            yPos += font->getSize() * font->getLineSpacing();
        }

        state.texts.push_back(TextRenderer::Text{word, {xPos, yPos}, color});
    }

    state.cursor.y += yPos - pos.y + 2*MARGIN;
}

bool Gui::button(std::string_view label) {
    ContentState& state = getCurrentState();
    Basic::Vec2 pos = transform(state.cursor);

    Basic::Vec2 textSize = font->measureText(label);
    Basic::Vec4 rect = {pos.x, pos.y, textSize.x + 2 * PADDING, textSize.y + 2 * PADDING};

    static int prevState = GLFW_RELEASE;
    int newState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    bool hovered = insideRect(mouse, rect);
    bool mouseDown = newState == GLFW_PRESS && prevState == GLFW_RELEASE;
    prevState = newState;

    Basic::Vec4 color = hovered? 
        (mouseDown ? Basic::hexColor(0xFFFF0000) : Basic::hexColor(0xFF00FF00))
        : Basic::hexColor(0xFFFFFFFF);

    state.quads.push_back(Renderer::Quad{rect, color, nullptr});

    float textX = pos.x + PADDING;
    float textY = pos.y + (rect.w + textSize.y) / 2.0f;
    state.texts.push_back(TextRenderer::Text{label, {textX, textY}, Basic::hexColor(0xFF000000)});

    state.cursor.y += rect.w + 2*MARGIN;

    return hovered && mouseDown;
}

Basic::Vec2 Gui::transform(const Basic::Vec2 point) {
    ContentState& state = getCurrentState();

    return {
        state.rect.x + point.x,
        state.rect.y + point.y + scrollData[state.id]
    };
}

Gui::ContentState& Gui::getCurrentState() {
    return contentStack[currenStateIndex];
}
