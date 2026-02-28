// Created by ari on 2/25/26.

#include "Game.h"

#include "Gui.h"
#include "Renderer.h"
#include "TextureLibrary.h"

Basic::Vec2 getFrameSize(GLFWwindow* window);

Game::Game() {
    storyEngine.loadStory();
}

void Game::update(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

void Game::render(GLFWwindow* window) {
    Basic::Vec2 frameSize = getFrameSize(window);

    if (!storyEngine.isDialogueFinished()) {
        const StoryEngine::Dialogue& dialogue = storyEngine.getCurrentDialogue();

        // Renderer renderer;
        // renderer.begin(frameRect);
        // {
        //     // Drawing background
        //     Texture& tex4 = TextureLibrary::getInstance().getTexture(dialogue.backgroundPath);
        //     Renderer::Quad background{frameRect, Basic::hexColor(0xFFFFFFFF), &tex4};
        //     renderer.submit(background);
        //
        //     // Drawing character
        //     Texture &tex1 = TextureLibrary::getInstance().getTexture(dialogue.characterSpritePath);
        //     float characterHeight = frameRect.w;
        //     float characterWidth = (characterHeight / tex1.getHeight()) * tex1.getWidth();
        //     float characterPosX = (dialogue.speakerPosition == StoryEngine::SpeakerPosition::RIGHT)? frameRect.z - characterWidth : 0.0f;
        //     Renderer::Quad character{{characterPosX, 0.0f, characterWidth, characterHeight}, Basic::hexColor(0xFFFFFFFF), &tex1};
        //     renderer.submit(character);
        // }
        // renderer.end();

        // GUI Layer
        // Basic::Vec4 panelRect = {0.0f, 2.0f * frameRect.w / 3.0f, frameRect.z, frameRect.w / 4.0f};

        Gui gui;
        gui.begin("win1", {0.0f, 0.0f, frameSize.x, frameSize.y/3.0f});
        {
            gui.button("Next");
            gui.button("Test Button");
            gui.text(dialogue.text, Basic::hexColor(0xFF000000));
            gui.button("Test Button");
            gui.button("Test Button");
            gui.text(dialogue.text, Basic::hexColor(0xFF000000));

            gui.button("Test Button");
            gui.button("Test Button");

            // gui.scrollBegin(0, (int)panelRect.y, (int)panelRect.z, (int)panelRect.w);
            // {
            //     float padding = 20.0f;
            //     Basic::Vec4 textRect = {panelRect.x + padding, panelRect.y + padding, panelRect.z - 2 * padding, panelRect.w - 2 * padding };
            //     gui.text(dialogue.text, textRect, Basic::hexColor(0xFF000000));
            // }
            // gui.scrollEnd();
            //
            // if (gui.button("Next")) {
            //     storyEngine.advance();
            // }
        }
        gui.end();

        gui.begin("win2", {0.0f, frameSize.y/3.0f, frameSize.x, frameSize.y/3.0f});
        {
            gui.text(dialogue.text, Basic::hexColor(0xFF000000));
            gui.button("Test long Button .........");
        }
        gui.end();

    } else {
        // Draw choices
    }
}

Basic::Vec2 getFrameSize(GLFWwindow* window) {
    int frameWidth, frameHeight;
    glfwGetFramebufferSize(window, &frameWidth, &frameHeight);
    return {static_cast<float>(frameWidth), static_cast<float>(frameHeight)};
}