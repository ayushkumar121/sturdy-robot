// Created by ari on 2/24/26.

#pragma once

#include <vector>

#include "Basic.h"
#include "Font.h"

class TextRenderer {
public:
	struct Text {
		std::string data;
		Basic::Vec2 pos;
		Basic::Color color;
	};

	void begin(Font* font, Basic::Vec4 rect);
	void submit(Text text);
	void end();
private:
	Font* font = nullptr;
	std::vector<Text> drawList;
	Basic::Mat4 projection;
};