#include "RocketSlingersController.hpp"
#include <iostream>

RocketSlingersController::RocketSlingersController() {
	initSdl();
	gameState = new GameState();
	initGameObjects();
}

void RocketSlingersController::initSdl() {

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::string sdlError = SDL_GetError();
		throw "RocketSlingersController::initSdl - SDL initialization failed: " + sdlError;
	}
	SDL_DisableScreenSaver();

}

void RocketSlingersController::initGameObjects() {
	poAxes = new PoAxes(gameState);
	poGuy = new PoGuy(gameState);
	poPendulum = new PoPendulum(gameState);
	poRope = new PoRope(gameState);
}

void RocketSlingersController::start() {
	bool running = true;
	while (running) {
		gameState->frameStart();
		running = gameState->inputQueue->processInput();
		updateGameState();
		gameState->physicalObjectRenderer->render();
		gameState->renderWindow->publishFrame();
		gameState->frameEnd();
	}
}

void RocketSlingersController::updateGameState() {
	poAxes->updatePhysicalState();
	poGuy->updatePhysicalState();
	poPendulum->updatePhysicalState();
	poRope->updatePhysicalState();

	poAxes->updateRenderState();
	poGuy->updateRenderState();
	poPendulum->updateRenderState();
	poRope->updateRenderState();
}

RocketSlingersController::~RocketSlingersController() {
	delete poAxes;
	delete poGuy;
	delete poPendulum;
	delete poRope;
	delete gameState;
	SDL_Quit();
}
