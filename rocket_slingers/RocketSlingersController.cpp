#include "RocketSlingersController.hpp"
#include <iostream>

RocketSlingersController::RocketSlingersController() {
	gameState = new GameState();
	initGameObjects();
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
		running = gameState->renderWindow->processInput();
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
}

RocketSlingersController::~RocketSlingersController() {
	delete poAxes;
	delete poGuy;
	delete poPendulum;
	delete poRope;
	delete gameState;
}
