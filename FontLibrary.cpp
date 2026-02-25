// Created by ari on 2/25/26.

#include "FontLibrary.h"

#include <iostream>

FontLibrary::FontLibrary() {
	// TODO: make the sizes based on screen size/window scaling
	fonts.try_emplace(
        FontType::PLAYFAIR,
        "assets/fonts/Playfair.ttf", 32
    );
    fonts.try_emplace(
        FontType::PRESS_START,
        "assets/fonts/PressStartRegular.ttf", 32
    );
}

FontLibrary::~FontLibrary() {
    std::cout << "Font Library Destroyed" << std::endl;
}

FontLibrary& FontLibrary::getInstance() {
	static FontLibrary instance;
	return instance;
}

const Font& FontLibrary::getFont(FontType type) const {
	return fonts.at(type);
}