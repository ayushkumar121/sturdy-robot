// Created by ari on 2/25/26.

#pragma once

#include "Basic.h"
#include "Gui.h"
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
	Screen screen = Screen::DESKTOP;
	SubScreen subScreen = SubScreen::NONE;
	SubScreen targetSubScreen = SubScreen::NONE;

	Basic::Vec2 frameSize{};
	float subScreenY;
	float targetSubScreenY;

	Gui gui;
	StoryEngine storyEngine;

	std::string currentTaskId = "task_01";

	void renderDesktop(float dt);
	void renderBackground(float dt);
	void renderSubScreen(float dt);
	void renderTaskBar(float dt);
};
