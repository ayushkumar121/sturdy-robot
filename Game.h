// Created by ari on 2/25/26.

#pragma once

#include "Basic.h"
#include "StoryEngine.h"
#include "TaskEngine.h"

class Game {
public:
	enum class Screen {
		DESKTOP,
		STORY,
		MENU,
	};

	enum class SubScreen {
		NONE,
		CHATBOT,
		EMAIL,
	};

	Game();
	void update(float dt);
	void render(float dt);
private:
	StoryEngine storyEngine;
	TaskEngine taskEngine;

	Screen screen = Screen::DESKTOP;
	SubScreen subScreen = SubScreen::NONE;

	float subScreenY{};

	void renderDesktop(float dt);
	void openSubScreen(Basic::Vec2 frameSize, SubScreen subScreen);
	void renderSubScreen(Basic::Vec2 frameSize);

	void renderTaskBar(Basic::Vec2 frameSize);
};
