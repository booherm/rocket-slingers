#include "RenderWindow.hpp"
#include <iostream>

GameState* RenderWindow::gameState;

RenderWindow::RenderWindow(GameState* gameState) {
	this->gameState = gameState;
	initGlWindow();
}

void RenderWindow::publishFrame() {
	SDL_GL_SwapWindow(glWindow);
}

void RenderWindow::initGlWindow() {

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	// get right most display
	unsigned int rightmostDisplay = 0;
	int rightmostDisplayX = 0;
	int rightmostDisplayY = 0;
	for (int i = 0; i < SDL_GetNumVideoDisplays(); ++i) {
		SDL_Rect displayBounds;
		SDL_GetDisplayBounds(i, &displayBounds);
		//std::cout << "Display " << i << " bounds: (" << displayBounds.w << ", " << displayBounds.h << ") : (" << displayBounds.x << ", " << displayBounds.y << ")" << std::endl;
		if (displayBounds.x > rightmostDisplayX) {
			rightmostDisplay = i;
			rightmostDisplayX = displayBounds.x;
			rightmostDisplayY = displayBounds.y;
		}
	}

	glWindow = SDL_CreateWindow("Rocket Slingers", rightmostDisplayX, rightmostDisplayY,
		gameState->resolutionWidth, gameState->resolutionHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN);

	glContext = SDL_GL_CreateContext(glWindow);
	SDL_GL_SetSwapInterval(1);

	int drawableWidth, drawableHeightH;
	SDL_GL_GetDrawableSize(glWindow, &drawableWidth, &drawableHeightH);
	glViewport(0, 0, drawableWidth, drawableHeightH);

	// Initialize GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		throw std::string("RenderWindow::initGlWindow - Failed to initialize GLEW");
	}
	glGetError();  // consume meaningless open gl error raised by glew initialization

	// enable anti-aliasing and color alpha blending
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	publishFrame();

	SDL_ShowWindow(glWindow);
}

RenderWindow::~RenderWindow() {
	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(glWindow);
}
