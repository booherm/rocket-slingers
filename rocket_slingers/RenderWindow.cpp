#include "RenderWindow.hpp"


RenderWindow::RenderWindow() {
	initGlWindow();
	initShaders();
}

void RenderWindow::doRender() {
}

void RenderWindow::initGlWindow() {

	if (!glfwInit()) {
		throw std::string("Failed to initialize GLFW");
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	glWindow = glfwCreateWindow(1920, 1080, "Rocket Slingers", nullptr, nullptr);
	glfwSetWindowPos(glWindow, 1920, 0);

	if (glWindow == nullptr) {
		glfwTerminate();
		throw std::string("Failed to create GLFW window");
	}

	glfwMakeContextCurrent(glWindow);
	glfwSetKeyCallback(glWindow, keyCallback);

	// Initialize GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		glfwTerminate();
		throw std::string("Failed to initialize GLEW");
	}

	glViewport(0, 0, 1920, 1080);

	// Ensure we can capture the escape key being pressed below
	//glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE); ????

	// enable face culling since we're only going to render in 2d
	glCullFace(GL_FRONT);  // this may end up backwards
	glEnable(GL_CULL_FACE);

	// enable color alpha blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void RenderWindow::initShaders() {

}

void RenderWindow::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, GL_TRUE);
			break;
		}
	}

}
