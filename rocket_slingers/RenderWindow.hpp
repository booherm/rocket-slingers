#ifndef RENDERWINDOW_HPP
#define RENDERWINDOW_HPP

#include <glew.h>
#include <glfw3.h>
#include "GameState.hpp"

class GameState;

class RenderWindow {

public:
	RenderWindow(GameState* gameState);
	~RenderWindow();
	void publishFrame();
	bool processInput();
	double getTimeSeconds();

private:
	GLFWwindow* glWindow;
	static GameState* gameState;

	void initGlWindow();
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int modifiers);
	static void cursorPositionCallback(GLFWwindow* window, double xPosition, double yPostion);
	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int modifiers);
	static void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);
	static void glfwErrorCallback(int error, const char* description);
};

#endif
