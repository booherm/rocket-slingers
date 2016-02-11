#include "RenderWindow.hpp"
#include <iostream>

GameState* RenderWindow::gameState;

RenderWindow::RenderWindow(GameState* gameState) {
	this->gameState = gameState;

	initGlWindow();
}

void RenderWindow::publishFrame() {
	glfwSwapBuffers(glWindow);
}

bool RenderWindow::processInput() {
	glfwPollEvents();

	if (glfwWindowShouldClose(glWindow)) {
		return false;
	}
	return true;
}

double RenderWindow::getTimeSeconds() {
	return glfwGetTime();
}

void RenderWindow::initGlWindow() {

	glfwSetErrorCallback(glfwErrorCallback);
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	// get right most monitor
	int monitorCount;
	int rightMostMonitorPosX;
	GLFWmonitor* rightMostMonitor = nullptr;
	GLFWmonitor** monitors = glfwGetMonitors(&monitorCount);
	for (int i = 0; i < monitorCount; i++) {
		GLFWmonitor* monitor = monitors[i];
		int mPosX, mPosY;
		glfwGetMonitorPos(monitor, &mPosX, &mPosY);

		if (rightMostMonitor == nullptr || mPosX > rightMostMonitorPosX) {
			rightMostMonitor = monitor;
			rightMostMonitorPosX = mPosX;
		}

		/*
		int mWidth, mHeight;
		glfwGetMonitorPhysicalSize(monitor, &mWidth, &mHeight);
		std::cout << "--------------------------" << std::endl;
		std::cout << "Monitor " << i << ":" << std::endl;
		std::cout << "            Name: " << glfwGetMonitorName(monitor) << std::endl;
		std::cout << "   Physical size: (" << mWidth << "x" << mHeight << ")" << std::endl;
		std::cout << "Virtual position: (" << mPosX << ", " << mPosY << ")" << std::endl;
		*/
	}

	// create window
	glWindow = glfwCreateWindow(gameState->resolutionWidth, gameState->resolutionHeight, "Rocket Slingers", rightMostMonitor, nullptr);
	glfwMakeContextCurrent(glWindow);
	int frameBufferWidth, frameBufferHeight;
	glfwGetFramebufferSize(glWindow, &frameBufferWidth, &frameBufferHeight);
	glViewport(0, 0, frameBufferWidth, frameBufferHeight);
	glfwSwapInterval(1);

	// setup window input
	glfwSetInputMode(glWindow, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetInputMode(glWindow, GLFW_STICKY_MOUSE_BUTTONS, GL_TRUE);
	//glfwSetInputMode(glWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetKeyCallback(glWindow, keyCallback);
	glfwSetCursorPosCallback(glWindow, cursorPositionCallback);
	glfwSetMouseButtonCallback(glWindow, mouseButtonCallback);
	glfwSetScrollCallback(glWindow, scrollCallback);

	// Initialize GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		glfwTerminate();
		throw std::string("Failed to initialize GLEW");
	}
	glGetError();  // consume meaningless open gl error raised by glew initialization

	// enable anti-aliasing and color alpha blending
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	publishFrame();
	glfwShowWindow(glWindow);
}

void RenderWindow::glfwErrorCallback(int error, const char* description) {
	throw std::to_string(error) + " - " + std::string(description);
}

void RenderWindow::keyCallback(GLFWwindow* window, int key, int scancode, int action, int modifiers) {

	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, GL_TRUE);
			break;
		}
	}

	double x, y;
	glfwGetCursorPos(window, &x, &y);
	gameState->inputQueue->addEvent(key, action, modifiers, x, y);
}

void RenderWindow::cursorPositionCallback(GLFWwindow* window, double xPosition, double yPostion) {
	double x, y;
	glfwGetCursorPos(window, &x, &y);
	gameState->inputQueue->addMouseMovementEvent(x, y);
}

void RenderWindow::mouseButtonCallback(GLFWwindow* window, int button, int action, int modifiers) {
	double x, y;
	glfwGetCursorPos(window, &x, &y);
	gameState->inputQueue->addEvent(button, action, modifiers, x, y);
}

void RenderWindow::scrollCallback(GLFWwindow* window, double xOffset, double yOffset){
	std::cout << "received scroll input" << std::endl;
}

void RenderWindow::getCurrentCursorPosition(double* x, double* y) {
	glfwGetCursorPos(glWindow, x, y);
}

RenderWindow::~RenderWindow() {
	glfwTerminate();
}
