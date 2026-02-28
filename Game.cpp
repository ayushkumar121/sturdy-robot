// Created by ari on 2/25/26.

#include "Game.h"

#include <iostream>

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

    Renderer renderer;
    Gui gui;

    renderer.begin(frameSize);
    {
        // Draw desktop background
        Basic::Vec4 frameRect = {0.0f, 0.0f, frameSize.x, frameSize.y};
        Texture &backgroundTex = TextureLibrary::getInstance().getTexture("assets/backgrounds/officedark.jpg");
        renderer.submit({frameRect, Basic::hexColor(0xFFFFFFFF), &backgroundTex});
        renderer.submit({frameRect, Basic::hexColor(0x55000000), nullptr});
    }
    renderer.end();

    switch (gameScreen) {
        case DESKTOP: {
            gui.begin("Desktop", {0.0f, frameSize.y * 0.8f, frameSize.x, frameSize.y});
            Texture &bot = TextureLibrary::getInstance().getTexture("assets/characters/cordova_bot.png");
            gui.image(&bot, {100.0f, 100.0f});
            if (gui.button("Chat bot")) {
                gameScreen = CHATBOT;
            }
            gui.end();
        }
        break;
        case CHATBOT: {
            Basic::Vec4 panelRect = {frameSize.x * 0.10f, frameSize.y * 0.10f, frameSize.x * 0.8f, frameSize.y * 0.8f};
            renderer.begin(frameSize);
            {
                renderer.submit({panelRect, Basic::hexColor(0xAAFFFFFF), nullptr});
            }
            renderer.end();

            std::string taskId = "task_01";
            TaskEngine::Task &task = taskEngine.getTask(taskId);

            Basic::Vec4 chatRect = {panelRect.x, panelRect.y, panelRect.z * 0.7f, panelRect.w};
            gui.begin("ChatWindow", chatRect);
            {
                if (gui.button("Close")) {
                    gameScreen = DESKTOP;
                }

                const TaskEngine::Message &message = task.getCurrentMessage();
                Texture &bot = TextureLibrary::getInstance().getTexture("assets/characters/cordova_bot.png");
                gui.image(&bot, {100.0f, 100.0f});
                gui.text(message.text, Basic::hexColor(0xFF666666));
                for (auto &choice: message.choices) {
                    if (gui.button(choice.text)) {
                        task.chooseReply(choice.nextMessageId);
                    };
                }
            }
            gui.end();

            Basic::Vec4 projectFileRect = {chatRect.x + chatRect.z, panelRect.y, panelRect.z * 0.3f, panelRect.w};
            gui.begin("ProjectFiles", projectFileRect);
            gui.text("Project Files", Basic::hexColor(0xFF666666));
            {
                for (auto &[_, file]: task.files) {
                    if (file.type == TaskEngine::FileType::IMAGE) {
                        Texture &tex = TextureLibrary::getInstance().getTexture(file.path);
                        float texWidth = panelRect.z * 0.15f;
                        float texHeight = texWidth * ((float) tex.getHeight() / (float) tex.getWidth());
                        gui.image(&tex, {texWidth, texHeight});
                    }
                    gui.button(file.name);
                }
            }
            gui.end();

            gui.begin("Submit", chatRect);
            gui.moveCursor({0.0f, chatRect.w * 0.9f});
            gui.button("Submit Current Files");
            gui.end();
        }
        break;
        default: break;
    }
}

Basic::Vec2 getFrameSize(GLFWwindow* window) {
    int frameWidth, frameHeight;
    glfwGetFramebufferSize(window, &frameWidth, &frameHeight);
    return {static_cast<float>(frameWidth), static_cast<float>(frameHeight)};
}