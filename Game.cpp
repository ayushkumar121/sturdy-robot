// Created by ari on 2/25/26.

#include "Game.h"

#include <iostream>

#include "Gui.h"
#include "TextureLibrary.h"
#include "FontLibrary.h"

Game::Game() {
    storyEngine.loadStory();

    // Rendering offscreen initally
    subScreenY = targetSubScreenY = 2000;
}

void Game::update(float dt) {
    GLFWwindow* window = glfwGetCurrentContext();
    gui.update(window);

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    int frameWidth, frameHeight;
    glfwGetFramebufferSize(window, &frameWidth, &frameHeight);

    frameSize.x = (float)frameWidth;
    frameSize.y = (float)frameHeight;

    if (subScreen == SubScreen::NONE) {
        targetSubScreenY = frameSize.y * 1.2f;
    }

    if (targetSubScreen != subScreen) {
        if (subScreenY < frameSize.y) {
            targetSubScreenY = frameSize.y * 1.2f;
        } else {
            subScreen = targetSubScreen;
            targetSubScreenY = frameSize.y * 0.05f;
        }
    }

    subScreenY += (targetSubScreenY-subScreenY) * 6.0f * dt;
}

void Game::render(float dt) {
    switch (screen) {
        case Screen::DESKTOP: renderDesktop(dt); break;
        case Screen::MENU: {} break;
        case Screen::STORY: {} break;
    }
}

void Game::renderDesktop(float dt) {
    renderBackground(dt);
    renderSubScreen(dt);
    renderTaskBar(dt);
}

void Game::renderBackground(float dt) {
    gui.begin("background", {0.0f, 0.0f, frameSize.x, frameSize.y});
    Texture &backgroundTex = TextureLibrary::getInstance().getTexture("assets/backgrounds/moonearth2.jpg");
    gui.image(&backgroundTex, frameSize);
    gui.end();
}

void Game::renderSubScreen(float dt) {
    Basic::Vec2 subScreenSize = {frameSize.x*0.8f, frameSize.y*0.8f};
    gui.begin("subscreen", {frameSize.x*0.1f, subScreenY, subScreenSize.x, subScreenSize.y});
    gui.rect(Basic::hexColor(0xAAFFFFFF), subScreenSize);
    gui.setCursor({0,0});
    float subScreenDecorationHeight = subScreenSize.y*0.05f;
    gui.rect(Basic::hexColor(0xAAFFFFFF), {subScreenSize.x, subScreenDecorationHeight});
    gui.setCursor({subScreenSize.x - subScreenDecorationHeight,0.0f});
    if (gui.imageButton(nullptr, {subScreenDecorationHeight, subScreenDecorationHeight})) {
        targetSubScreen = SubScreen::NONE;
    }
    
    gui.setCursor({subScreenSize.x/2.0f, subScreenDecorationHeight*0.15f});

    switch (subScreen) {
        case SubScreen::EMAIL: {
            gui.text("Email", Basic::hexColor(0xFF666666));
        } break;

        case SubScreen::CHATBOT: {
            gui.text("Chatbot", Basic::hexColor(0xFF666666));

            auto& task = TaskEngine::getInstance().getTask(currentTaskId);
            auto& message = task.getCurrentMessage();

            gui.setCursor({30.0f, 150.0f});
            Texture &botTex = TextureLibrary::getInstance().getTexture("assets/characters/cordova_bot.png");
            if (gui.imageButton(&botTex, {150.0f, 150.0f})) {
                task.reset();
            }
            gui.text(message.text, Basic::hexColor(0xFF666666));

            for (auto& choice: message.choices) {
                if (gui.button(choice.text)) {
                    if (choice.nextMessageId != "") // SHould be an assert
                    task.chooseReply(choice.nextMessageId);
                }
            }

            if (message.files.size() > 0) {
                gui.text("Generated Files", Basic::hexColor(0xFF666666));

                for (auto& [_, file]: message.files) {
                    if (file.type == TaskEngine::FileType::IMAGE) {
                        Texture &fileTex = TextureLibrary::getInstance().getTexture(file.path);
                        float imageWidth = 200.0f;
                        float imageHeight = imageWidth * ((float)fileTex.getHeight()/(float)fileTex.getWidth());
                        if (gui.imageButton(&fileTex, {imageWidth, imageHeight})) {
                            // View File
                        }
                    }
                }
            }

        } break;

        case SubScreen::NONE: break;
    }

    gui.end();
}

void Game::renderTaskBar(float dt) {
    gui.begin("taskbar", {0.0f, frameSize.y * 0.9f, frameSize.x, frameSize.y*0.1f});
    gui.rect(Basic::hexColor(0xAA000000), {frameSize.x, frameSize.y*0.1f});

    const float margin = 20.0f;
    const float buttonHeight = frameSize.y * 0.07f;
    const Basic::Vec2 buttonSize = {buttonHeight, buttonHeight};

    Texture &botTex = TextureLibrary::getInstance().getTexture("assets/sprites/robot.png");
    Texture &emailText = TextureLibrary::getInstance().getTexture("assets/sprites/mail.png");

    gui.setCursor({margin, 0.0f});

    if (gui.imageButton(&emailText, buttonSize)) {
        targetSubScreen = SubScreen::EMAIL;
    }

    gui.setCursor({buttonSize.x + 2*margin, 0.0f});

    if (gui.imageButton(&botTex, buttonSize)) {
        targetSubScreen = SubScreen::CHATBOT;
    }
    gui.end();
}
