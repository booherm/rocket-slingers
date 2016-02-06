#include "RocketSlingersController.hpp"
#include <iostream>

RocketSlingersController::RocketSlingersController() {
	gameState = new GameState();
	initGameObjects();
	std::cout << "controller constructed" << std::endl;
}

void RocketSlingersController::initGameObjects() {
	pt = new PoPendulum(gameState);
	axes = new PoAxes(gameState);
}

void RocketSlingersController::start() {
	std::cout << "controller start" << std::endl;
	bool running = true;
	while (running) {
		gameState->frameStart();
		running = gameState->renderWindow->processInput();
		updateGameState();
		gameState->renderWindow->publishFrame();
	}
}

void RocketSlingersController::updateGameState() {
	// clear current frame
	glClear(GL_COLOR_BUFFER_BIT);
	axes->updatePhysicalState();
	axes->render();
	pt->updatePhysicalState();
	pt->render();
}

RocketSlingersController::~RocketSlingersController() {
	delete pt;
	delete axes;
	delete gameState;

	std::cout << "controller destructed" << std::endl;
}
