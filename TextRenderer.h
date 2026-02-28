// Created by ari on 2/24/26.

#pragma once

#include <string_view>
#include <vector>

#include "Basic.h"
#include "Font.h"

class TextRenderer {
public:
	struct Text {
		std::string_view data;
		Basic::Vec2 pos;
		Basic::Color color;
	};

	void begin(const Font *font, Basic::Vec2 frameSize);
	void submit(Text text);
	void end();
private:
	const Font* font = nullptr;
	std::vector<Text> drawList;
	Basic::Mat4 projection{};
};