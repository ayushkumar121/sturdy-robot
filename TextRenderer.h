// Created by ari on 2/24/26.

#pragma once

#include <cstdint>
#include <vector>

#include "Basic.h"

class TextRenderer {
public:
	struct Glyph {
		uint32_t textureId;
		Basic::Vec2 pos;
		Basic::Vec2 size;
		Basic::Color color;
	};

	void begin(Basic::Vec4 rect);
	void submit(Glyph glyph);
	void end();
private:
	std::vector<Glyph> drawList;
	Basic::Mat4 projection;
};