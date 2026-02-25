// Created by ari on 2/25/26.

#pragma once

#include <unordered_map>

#include "Font.h"

class FontLibrary {
public:
	enum class FontType {
		PLAYFAIR,
		PRESS_START
	};
	static FontLibrary& getInstance();
	const Font& getFont(FontType type) const;
	~FontLibrary();

private:
	FontLibrary();
	std::unordered_map<FontType, Font> fonts;
};