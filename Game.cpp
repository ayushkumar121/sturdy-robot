// Created by ari on 2/25/26.

#include "Game.h"

#include <iostream>

#include "Gui.h"
#include "TextureLibrary.h"

Basic::Vec2 getFrameSize() {
    int frameWidth, frameHeight;
    glfwGetFramebufferSize(glfwGetCurrentContext(), &frameWidth, &frameHeight);
    return {static_cast<float>(frameWidth), static_cast<float>(frameHeight)};
}

Game::Game() {
    storyEngine.loadStory();
    subScreenY = getFrameSize().y;
}

void Game::update(float dt) {
    GLFWwindow* window = glfwGetCurrentContext();
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

void Game::render(float dt) {
    switch (screen) {
        case Screen::DESKTOP: renderDesktop(dt); break;
        case Screen::MENU: {} break;
        case Screen::STORY: {} break;
    }
}

void Game::renderDesktop(float dt) {
    const Basic::Vec2 frameSize = getFrameSize();
    renderSubScreen(frameSize);
    renderTaskBar(frameSize);
}

void Game::openSubScreen(Basic::Vec2 frameSize, SubScreen subScreen) {
    switch (subScreen) {
        case SubScreen::CHATBOT: break;
        case SubScreen::EMAIL: break;
        case SubScreen::NONE: break;
    }
}

void Game::renderSubScreen(Basic::Vec2 frameSize) {
    switch (subScreen) {
        case SubScreen::CHATBOT: break;
        case SubScreen::EMAIL: break;
        case SubScreen::NONE: break;
    }
}

void Game::renderTaskBar(Basic::Vec2 frameSize) {
    Gui gui;
    gui.begin("taskbar", {0.0f, 0.0f, frameSize.x, frameSize.y});
    Texture &backgroundTex = TextureLibrary::getInstance().getTexture("assets/backgrounds/moonearth2.jpg");
    gui.setMargin(0.0f);
    gui.image(&backgroundTex, {frameSize.x, frameSize.y});
    gui.moveCursor(0, frameSize.y * 0.9f);
    gui.rect(Basic::hexColor(0xAAFFFFFF), {frameSize.x, frameSize.y*0.1f});
    gui.setMargin(10.0f);
    gui.moveCursor(0, frameSize.y * 0.9f);

    const float margin = 20.0f;
    const float buttonHeight = frameSize.y * 0.07f;
    const Basic::Vec2 buttonSize = {buttonHeight, buttonHeight};

    Texture &botTex = TextureLibrary::getInstance().getTexture("assets/sprites/robot.png");
    Texture &emailText = TextureLibrary::getInstance().getTexture("assets/sprites/mail.png");

    if (gui.imageButton(&botTex, buttonSize)) {
        openSubScreen(frameSize, SubScreen::CHATBOT);
    }

    gui.moveCursor(buttonSize.x + margin, frameSize.y * 0.9f);

    if (gui.imageButton(&emailText, buttonSize)) {
        openSubScreen(frameSize, SubScreen::EMAIL);
    }
    gui.end();
}