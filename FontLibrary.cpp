// Created by ari on 2/25/26.

#include "FontLibrary.h"

#include <iostream>

#include "GLFW/glfw3.h"

static int determineFontSize() {
    GLFWwindow* window = glfwGetCurrentContext();
    int frameWidth, frameHeight;
    glfwGetFramebufferSize(window, &frameWidth, &frameHeight);

    return frameHeight/45;
}

FontLibrary::FontLibrary() {
    int fontSize = determineFontSize();
    fonts.try_emplace(
        FontType::ROBOTO,
        "assets/fonts/Roboto.ttf", fontSize
    );
	fonts.try_emplace(
        FontType::PLAYFAIR,
        "assets/fonts/Playfair.ttf", fontSize
    );
    fonts.try_emplace(
        FontType::PRESS_START,
        "assets/fonts/PressStartRegular.ttf", fontSize
    );
}

FontLibrary::~FontLibrary() {
    std::cerr << "Font Library Destroyed" << std::endl;
}

FontLibrary& FontLibrary::getInstance() {
	static FontLibrary instance;
	return instance;
}

const Font& FontLibrary::getFont(FontType type) const {
	return fonts.at(type);
}