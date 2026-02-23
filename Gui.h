#ifndef STURDY_ROBOT_GUI_H
#define STURDY_ROBOT_GUI_H

#include <GLFW/glfw3.h>

#include "Basic.h"
#include "Renderer.h"

class Gui {
public:
	void begin(GLFWwindow *window);
	void end();

	bool button(Basic::Vec4 rect);
private:
	GLFWwindow *window = nullptr;
	Renderer renderer;
};

#endif //STURDY_ROBOT_GUI_H