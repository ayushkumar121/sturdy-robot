// Created by ari on 2/25/26.

#pragma once

#include <GLFW/glfw3.h>

#include "StoryEngine.h"
#include "TaskEngine.h"

class Game {
public:
	enum GameScreen {
		DESKTOP,
		CHATBOT,
		EMAIL,
		CONVERSATION,
	};

	Game();
	void update(GLFWwindow* window);
	void render(GLFWwindow* window);
private:
	StoryEngine storyEngine;
	TaskEngine taskEngine;
	std::string taskId = "task_01";
	GameScreen gameScreen = DESKTOP;
};
