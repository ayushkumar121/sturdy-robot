// Created by ari on 2/25/26.

#include "Game.h"

#include <iostream>

#include "Renderer.h"
#include "TextRenderer.h"
#include "FontLibrary.h"
#include "TextureLibrary.h"
#include "Gui.h"

Basic::Vec4 getFrameRect(GLFWwindow* window);

Game::Game() {
    storyEngine.loadStory();
}
Game::~Game() {}

void Game::update(GLFWwindow* window) {

}

void Game::render(GLFWwindow* window) {
    Basic::Vec4 frameRect = getFrameRect(window);

    if (!storyEngine.isDialogueFinished()) {
        const StoryEngine::Dialogue& dialogue = storyEngine.getCurrentDialogue();

        Renderer renderer;
        renderer.begin(frameRect);

        // Drawing background
        Texture& tex4 = TextureLibrary::getInstance().getTexture(dialogue.backgroundPath);
        Renderer::Quad background{frameRect, Basic::hexColor(0xFFFFFFFF), &tex4};
        renderer.submit(background);

        // Drawing character
        Texture &tex1 = TextureLibrary::getInstance().getTexture(dialogue.characterSpritePath);
        float characterHeight = frameRect.w;
        float characterWidth = (characterHeight / tex1.getHeight()) * tex1.getWidth();
        Renderer::Quad character{{0.0f, 0.0f, characterWidth, characterHeight}, Basic::hexColor(0xFFFFFFFF), &tex1};
        renderer.submit(character);

        Basic::Vec4 panelRect = {0.0f, 3.0f * frameRect.w / 4.0f, frameRect.z, frameRect.w / 4.0f};
        Renderer::Quad panel{panelRect, Basic::hexColor(0x55FFFFFF), nullptr};
        renderer.submit(panel);
        renderer.end();

        // GUI Layer
        Gui gui;
        gui.begin(window);
        gui.text(dialogue.text, panelRect, Basic::hexColor(0xFF000000));
        if (gui.button("Next", {frameRect.z - 200.0f, frameRect.w - 100.0f})) {
            storyEngine.advance();
        }
        gui.end();
    } else {
        // Draw choices
    }
}

Basic::Vec4 getFrameRect(GLFWwindow* window) {
    int frameWidth, frameHeight;
    glfwGetFramebufferSize(window, &frameWidth, &frameHeight);
    return {0.0f, 0.0f, static_cast<float>(frameWidth), static_cast<float>(frameHeight)};
}