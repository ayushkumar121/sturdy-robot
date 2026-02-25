// Created by ari on 2/25/26.

#include "Game.h"

#include "Renderer.h"
#include "TextureLibrary.h"

Basic::Vec4 getFrameRect(GLFWwindow* window);

Game::Game() {
    storyEngine.loadStory();
}

void Game::update(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

void Game::render(GLFWwindow* window) {
    Basic::Vec4 frameRect = getFrameRect(window);

    if (!storyEngine.isDialogueFinished()) {
        const StoryEngine::Dialogue& dialogue = storyEngine.getCurrentDialogue();

        Renderer renderer;
        renderer.begin(frameRect);
        {
            // Drawing background
            Texture& tex4 = TextureLibrary::getInstance().getTexture(dialogue.backgroundPath);
            Renderer::Quad background{frameRect, Basic::hexColor(0xFFFFFFFF), &tex4};
            renderer.submit(background);

            // Drawing character
            Texture &tex1 = TextureLibrary::getInstance().getTexture(dialogue.characterSpritePath);
            float characterHeight = frameRect.w;
            float characterWidth = (characterHeight / tex1.getHeight()) * tex1.getWidth();
            float characterPosX = (dialogue.speakerPosition == StoryEngine::SpeakerPosition::RIGHT)? frameRect.z - characterWidth : 0.0f;
            Renderer::Quad character{{characterPosX, 0.0f, characterWidth, characterHeight}, Basic::hexColor(0xFFFFFFFF), &tex1};
            renderer.submit(character);
        }
        renderer.end();

        // GUI Layer
        Basic::Vec4 panelRect = {0.0f, 2.0f * frameRect.w / 3.0f, frameRect.z, frameRect.w / 4.0f};

        gui.begin();
        {
            gui.scrollBegin(0, (int)panelRect.y, (int)panelRect.z, (int)panelRect.w);
            {
                float padding = 20.0f;
                Basic::Vec4 textRect = {panelRect.x + padding, panelRect.y + padding, panelRect.z - 2 * padding, panelRect.w - 2 * padding };
                gui.text(dialogue.text, textRect, Basic::hexColor(0xFF000000));
            }
            gui.scrollEnd();

            if (gui.button("Next", {frameRect.z - 500.0f, frameRect.w - 100.0f})) {
                storyEngine.advance();
            }
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