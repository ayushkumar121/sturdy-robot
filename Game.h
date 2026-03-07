// Created by ari on 2/25/26.

#pragma once

#include "Basic.h"
#include "Gui.h"

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
	Gui gui;

	Screen screen = Screen::DESKTOP;
	SubScreen subScreen = SubScreen::NONE;
	SubScreen targetSubScreen = SubScreen::NONE;

	Basic::Vec2 frameSize{};
	float subScreenY = 2000.0f;
	float targetSubScreenY = 2000.0f;
    float subScreenX{};

	float subScreenWidth = 0.0f;
	float targetSubScreenWidth = 0.0f;
	float subScreenHeight{};
	float subScreenDecorationHeight{};

	std::string currentTaskId = "task_01";

	void renderDesktop();
	void renderBackground();
	void renderSubScreen();
	void renderTaskBar();
	void renderSubScreenContent();
};
