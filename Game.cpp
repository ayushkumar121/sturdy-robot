// Created by ari on 2/25/26.

#include "Game.h"

#include <iostream>

#include "Gui.h"
#include "TextureLibrary.h"
#include "FontLibrary.h"
#include "EmailSystem.h"
#include "TaskSystem.h"
#include "Camera.h"

Game::Game() {
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
        targetSubScreenWidth = 0.0f;
    }

    if (targetSubScreen != subScreen) {
        if (subScreenY < frameSize.y) { // Going down case
            targetSubScreenY = frameSize.y * 1.2f;
            targetSubScreenWidth = 0.0f;
        } else {
            subScreen = targetSubScreen; // Going up case
            targetSubScreenY = frameSize.y * 0.05f;
            targetSubScreenWidth = frameSize.x * 0.8f;
        }
    }

    float factor = 8.0f;
    subScreenY += (targetSubScreenY-subScreenY) * factor * dt;
    subScreenWidth += (targetSubScreenWidth-subScreenWidth) * factor * dt;

    subScreenHeight = frameSize.y*0.8f;
    subScreenX = std::lerp(frameSize.x*0.50f,frameSize.x*0.10f,subScreenWidth/(frameSize.x*0.8f));
    subScreenDecorationHeight = subScreenHeight*0.05f;

    Camera::getInstance().update(dt);
}

void Game::render(float dt) {
    switch (screen) {
        case Screen::DESKTOP: renderDesktop(); break;
        case Screen::MENU: {} break;
        case Screen::STORY: {} break;
    }
}

void Game::renderDesktop() {
    renderBackground();
    renderSubScreen();
    renderTaskBar();
}

void Game::renderBackground() {
    gui.begin("background", {0.0f, 0.0f, frameSize.x, frameSize.y}, false);
    Texture &backgroundTex = TextureLibrary::getInstance().getTexture("assets/backgrounds/moonearth2.jpg");
    gui.setMargin(0.0f);
    gui.image(&backgroundTex, frameSize);
    gui.setCursor({0.0f, 0.0f});
    if (gui.button("Shake")) {
        Camera::getInstance().shake(1.0f, 10.0f);
    }
    gui.end();
}

void Game::renderSubScreen() {
    gui.begin("subscreen", {subScreenX, subScreenY, subScreenWidth, subScreenHeight}, false);
    gui.rect(Basic::hexColor(0xAAFFFFFF), {subScreenWidth, subScreenHeight});
    gui.setCursor({0,0});

    gui.rect(Basic::hexColor(0xAAFFFFFF), {subScreenWidth, subScreenDecorationHeight});
    gui.setCursor({subScreenWidth - subScreenDecorationHeight,0.0f});
    if (gui.imageButton(nullptr, {subScreenDecorationHeight, subScreenDecorationHeight})) {
        targetSubScreen = SubScreen::NONE;
    }
    
    gui.setCursor({subScreenWidth/2.0f, subScreenDecorationHeight*0.15f});
            gui.text("Window", Basic::hexColor(0xFF666666));

    gui.end();

    renderSubScreenContent();
}

void Game::renderSubScreenContent() {
    switch (subScreen) {
        case SubScreen::EMAIL: {
            gui.begin("subscreen#email", {subScreenX, subScreenY+subScreenDecorationHeight, subScreenWidth/3.0f, subScreenHeight-subScreenDecorationHeight});
            {
                float marginLeft = 100.0f; 
                float marginTop = subScreenDecorationHeight; 
                float emailCardHeight = 300.0f; 
                float emailCardWidth = subScreenWidth/3.0f; 
                float emailCardSpacing = emailCardHeight*0.10f; 
                float emailCardTextSpacing = emailCardHeight*0.20f; 
                auto& emails = EmailSystem::getInstance().getEmails();
                int i = 0;

                EmailSystem::Email* selectedEmail = nullptr;
                for (auto& email : emails) {
                    gui.setCursor({50.0f, i*(emailCardHeight+emailCardSpacing) + marginTop});
                    gui.rect(Basic::hexColor(0xAAFFFFFF), {emailCardWidth, emailCardHeight});
                    gui.setCursor({marginLeft, i*(emailCardHeight+emailCardSpacing) + emailCardTextSpacing + marginTop});
                    gui.text(email.from);
                    gui.text(email.subject);

                    // if (email.taskId.has_value()) {
                    //     if (gui.button("OpenTask")) {
                    //         currentTaskId = *email.taskId;
                    //         targetSubScreen = SubScreen::CHATBOT;
                    //     }
                    // }
                    i++;
                }
            }
            gui.end();
            gui.begin("subscreen#emailbody", {subScreenX + subScreenWidth/3.0f + 50.0f, subScreenY+subScreenDecorationHeight, (subScreenWidth/3.0f)*2.0f, subScreenHeight-subScreenDecorationHeight});
            {
                gui.text("content");
            }
            gui.end();
        } break;

        case SubScreen::CHATBOT: {
            gui.begin("subscreen#chatbot", {subScreenX, subScreenY+subScreenDecorationHeight, subScreenWidth, subScreenHeight-subScreenDecorationHeight});
            {
                auto& task = TaskSystem::getInstance().getTask(currentTaskId);
                auto& message = task.getCurrentMessage();

                gui.setCursor({30.0f, subScreenDecorationHeight});
                Texture &botTex = TextureLibrary::getInstance().getTexture("assets/characters/cordova_bot.png");
                if (gui.imageButton(&botTex, {150.0f, 150.0f})) {
                    task.reset();
                }
                gui.text(message.text, Basic::hexColor(0xFF666666));

                for (auto& choice: message.choices) {
                    if (gui.button(choice.text)) {
                        if (choice.nextMessageId != "") // Should be an assert
                        task.chooseReply(choice.nextMessageId);
                    }
                }

                if (message.files.size() > 0) {
                    gui.text("Generated Files", Basic::hexColor(0xFF666666));

                    for (auto& [_, file]: message.files) {
                        if (file.type == TaskSystem::FileType::IMAGE) {
                            Texture &fileTex = TextureLibrary::getInstance().getTexture(file.path);
                            float imageWidth = 200.0f;
                            float imageHeight = imageWidth * ((float)fileTex.getHeight()/(float)fileTex.getWidth());
                            if (gui.imageButton(&fileTex, {imageWidth, imageHeight})) {
                                // View File
                            }
                        }
                    }
                }
            }
            gui.end();
        } break;

        case SubScreen::NONE: break;
    }
}

void Game::renderTaskBar() {
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
