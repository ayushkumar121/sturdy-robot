// Created by ari on 2/25/26.

#pragma once

#include <GLFW/glfw3.h>

class Game {
public:
	Game();
	~Game();
	void update(GLFWwindow* window);
	void render(GLFWwindow* window);
private:
};