#include "RocketSlingersController.hpp"
#include <iostream>
#include "Stage.hpp"

RocketSlingersController::RocketSlingersController() {
	initSdl();
	gameState = new GameState();
//	gameState->audioManager->playTest();
}

void RocketSlingersController::initSdl() {

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		std::string sdlError = SDL_GetError();
		throw "RocketSlingersController::initSdl - SDL initialization failed: " + sdlError;
	}
	SDL_DisableScreenSaver();

}

void RocketSlingersController::start() {
	bool running = true;
	while (running) {
		gameState->frameStart();
		running = gameState->eventBus->processInput();
		gameState->activeStage->update();
		gameState->physicalObjectRenderer->render();
		gameState->renderWindow->publishFrame();
		gameState->frameEnd();
	}
}

RocketSlingersController::~RocketSlingersController() {
	delete gameState;
	SDL_Quit();
}
