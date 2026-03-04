// Created by ari on 2/25/26.

#pragma once

#include <unordered_map>

#include "Font.h"

class FontLibrary {
public:
	enum class FontType {
		ROBOTO,
		ROBOTO_LARGE,
		PLAYFAIR,
		PLAYFAIR_LARGE,
		PRESS_START,
		PRESS_START_LARGE
	};
	static FontLibrary& getInstance();
	const Font& getFont(FontType type) const;
	~FontLibrary();

private:
	FontLibrary();
	std::unordered_map<FontType, Font> fonts;
};