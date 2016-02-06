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
	//GLFW_SAMPLES ?

	// debug - hardcoded resolution and monitor for now
	const unsigned int width = 1920;
	const unsigned int height = 1080;
	int frameBufferWidth;
	int frameBufferHeight;
	glWindow = glfwCreateWindow(width, height, "Rocket Slingers", nullptr, nullptr);
	glfwMakeContextCurrent(glWindow);
	glfwSetWindowPos(glWindow, width, 0);
	glfwGetFramebufferSize(glWindow, &frameBufferWidth, &frameBufferHeight);
	glfwSwapInterval(1);

	// setup window input
	glfwSetInputMode(glWindow, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetInputMode(glWindow, GLFW_STICKY_MOUSE_BUTTONS, GL_TRUE);
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

	glViewport(0, 0, frameBufferWidth, frameBufferHeight);
	//glOrtho((float) width / height, -((float) width / height), -1.0f, 1.0f, 0.8f, 100.0f);
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//	glOrtho(0.0f, (float) width / height, -((float) width / height), 0.0f, 0.0f, 1.0f);
//	float windowAspectRation = (float) width / height;
//	glOrtho(-windowAspectRation, windowAspectRation, -1.0f, 1.0f, 0.8f, 100.0f);

	// enable face culling since we're only going to render in 2d
	//glCullFace(GL_FRONT);  // this may end up backwards
	//glEnable(GL_CULL_FACE);

	// enable color alpha blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	//glFrustum(0.0f, 1.0f, 0.0f, 1.0f, 1.0f, -1.0f);
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
	//std::cout << "received cursor input" << std::endl;
}

void RenderWindow::mouseButtonCallback(GLFWwindow* window, int button, int action, int modifiers) {
	double x, y;
	glfwGetCursorPos(window, &x, &y);
	gameState->inputQueue->addEvent(button, action, modifiers, x, y);
}

void RenderWindow::scrollCallback(GLFWwindow* window, double xOffset, double yOffset){
	std::cout << "received scroll input" << std::endl;
}

RenderWindow::~RenderWindow() {
	glfwTerminate();
}
