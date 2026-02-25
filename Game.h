// Created by ari on 2/25/26.

#pragma once

#include <GLFW/glfw3.h>

#include "Gui.h"
#include "StoryEngine.h"

class Game {
public:
	Game();
	void update(GLFWwindow* window);
	void render(GLFWwindow* window);
private:
	StoryEngine storyEngine;
	Gui gui;
};
