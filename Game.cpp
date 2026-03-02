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
        Texture &backgroundTex = TextureLibrary::getInstance().getTexture("assets/backgrounds/pixeloffice.jpg");
        renderer.submit({frameRect, Basic::hexColor(0xFFFFFFFF), &backgroundTex});
        renderer.submit({frameRect, Basic::hexColor(0x55000000), nullptr});
    }
    renderer.end();

    switch (gameScreen) {
        case DESKTOP: {
            gui.begin("Desktop", {0.0f, frameSize.y * 0.8f, frameSize.x, frameSize.y});
            {
                gui.moveCursor({0.0f, 0.0f});
                gui.rect(Basic::hexColor(0xAAFFFFFF), {frameSize.x, frameSize.y});
                Basic::Vec2 buttonSize = {150.0f, 150.0f};
                gui.moveCursor({0.0f, 0.0f});
                Texture &mail = TextureLibrary::getInstance().getTexture("assets/sprites/mail.png");
                if (gui.imageButton(&mail, buttonSize)) {
                    gameScreen = EMAIL;
                }
                Texture &bot = TextureLibrary::getInstance().getTexture("assets/sprites/robot.png");
                gui.moveCursor({buttonSize.x*1.3f, 0.0f});
                if (gui.imageButton(&bot, buttonSize)) {
                    gameScreen = CHATBOT;
                }
            }
            gui.end();
        }
        break;
        case EMAIL: {
            Basic::Vec4 panelRect = {frameSize.x * 0.10f, frameSize.y * 0.10f, frameSize.x * 0.8f, frameSize.y * 0.8f};
            renderer.begin(frameSize);
            {
                renderer.submit({panelRect, Basic::hexColor(0xAAFFFFFF), nullptr});
            }
            renderer.end();

            gui.begin("EmailWindow", panelRect);
            {
                if (gui.button("Close")) {
                    gameScreen = DESKTOP;
                }
                Basic::Vec2 cursor = gui.getCursor();
                gui.rect(Basic::hexColor(0xAAFFFFFF), {panelRect.z * 0.8f, panelRect.w * 0.3f});
                gui.moveCursor(cursor);
                gui.text("Lorem ipsum dolor sit amet, consectetur"
                    "adipiscing elit. Pellentesque sem arcu, pretium vel est feugiat," 
                    "finibus elementum diam. Curabitur vitae augue efficitur,"
                    "euismod eros non, accumsan diam. Aenean scelerisque lacus felis,"
                    "ut vehicula lectus rhoncus id.", Basic::hexColor(0xFF000000));
                if (gui.button("Open Chatbot")) {
                    std::string taskId = "task_01";
                    gameScreen = CHATBOT;
                }
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

            TaskEngine::Task &task = taskEngine.getTask(taskId);

            Basic::Vec4 chatRect = {panelRect.x, panelRect.y, panelRect.z * 0.7f, panelRect.w};
            gui.begin("ChatWindow", chatRect);
            {
                if (gui.button("Close")) {
                    gameScreen = DESKTOP;
                }
                if (gui.button("Reset")) {
                    task.reset();
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