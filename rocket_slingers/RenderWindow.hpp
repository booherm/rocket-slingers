#ifndef RENDERWINDOW_HPP
#define RENDERWINDOW_HPP

#include <glew.h>
#include <glfw3.h>
#include <iostream>

//const GLuint GL_WINDOW_WIDTH = 1920;
//const GLuint GL_WINDOW_HEIGHT = 1080;

class RenderWindow {

public:
	RenderWindow();
	void doRender();

private:

	// OpenGL variables
	GLFWwindow* glWindow;

	// member functions
	void initGlWindow();
	void initShaders();
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
};

#endif
