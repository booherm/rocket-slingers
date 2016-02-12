#ifndef RENDERWINDOW_HPP
#define RENDERWINDOW_HPP

#include <glew.h>
#include <SDL.h>
#include "GameState.hpp"

class GameState;

class RenderWindow {

public:
	RenderWindow(GameState* gameState);
	~RenderWindow();
	void publishFrame();

private:
	static GameState* gameState;
	SDL_Window* glWindow;
	SDL_GLContext glContext;
	void initGlWindow();
};

#endif
