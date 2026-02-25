// Created by ari on 2/25/26.

#include "Game.h"

#include <iostream>

#include "Renderer.h"
#include "TextRenderer.h"
#include "FontLibrary.h"
#include "TextureLibrary.h"
#include "Gui.h"

Game::Game() {}
Game::~Game() {}

void Game::update(GLFWwindow* window) {

}

void Game::render(GLFWwindow* window) {
    // Basic::getScreenRect
    int frameWidth, frameHeight;
    glfwGetFramebufferSize(window, &frameWidth, &frameHeight);

    Basic::Vec4 screenRect{0.0f, 0.0f, (float)frameWidth, (float)frameHeight};
 
    // Normal Rendering
    Renderer renderer;
    renderer.begin(screenRect);

    Texture& tex4 = TextureLibrary::getInstance().getTexture("assets/officedark.jpg");
    Renderer::Quad background{screenRect, Basic::hexColor(0xFFFFFFFF), &tex4};
    renderer.submit(background);

    Texture& tex1 = TextureLibrary::getInstance().getTexture("assets/VN_Visiter_C.png");
    float characterHeight = (float)frameHeight;
    float characterWidth = (characterHeight/tex1.getHeight()) * tex1.getWidth();
    Renderer::Quad character{{0.0f, 0.0f, characterWidth, characterHeight}, Basic::hexColor(0xFFFFFFFF), &tex1};
    renderer.submit(character);

    Renderer::Quad panel{{0.0f, 3.0f*(float)frameHeight/4.0f, (float)frameWidth, (float)frameHeight/4.0f}, Basic::hexColor(0x55FFFFFF), nullptr};
    renderer.submit(panel);

    renderer.end();

    const Font& font = FontLibrary::getInstance().getFont(FontLibrary::FontType::PLAYFAIR);

    TextRenderer textRenderer;
    textRenderer.begin(&font, screenRect);
    std::string str = "In a hole in the ground there lived a hobbit.\n"
    "Not a nasty, dirty, wet hole, filled with the ends of worms and an oozy smell,\n"
    "nor yet a dry, bare, sandy hole with nothing in it to sit down on or to eat:\n"
    "it was a hobbit- hole, and that means comfort.";

    textRenderer.submit({str, {100.0f, 100.0f + 3.0f*(float)frameHeight/4.0f}, Basic::hexColor(0xFF000000)});
    textRenderer.end();

    // GUI Layer
    Gui gui;
    gui.begin(window);
    if (gui.button("Next", {3.0f*(float)frameWidth/4.0f, frameHeight - 100.0f})) {
        static int times = 0;
        std::cout << "Clicked " << times++ << std::endl;
    }
    gui.end();
}
