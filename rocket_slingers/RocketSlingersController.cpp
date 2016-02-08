#include "RocketSlingersController.hpp"
#include <iostream>

RocketSlingersController::RocketSlingersController() {
	gameState = new GameState();
	initGameObjects();
	std::cout << "controller constructed" << std::endl;
}

void RocketSlingersController::initGameObjects() {
	pg = new PoGuy(gameState);
	pendulum = new PoPendulum(gameState);
	axes = new PoAxes(gameState);
	rope = new PoRope(gameState);
}

void RocketSlingersController::start() {
	std::cout << "controller start" << std::endl;
	bool running = true;
	while (running) {
		gameState->frameStart();
		running = gameState->renderWindow->processInput();
		updateGameState();
		gameState->renderWindow->publishFrame();
		gameState->frameEnd();
	}
}

void RocketSlingersController::updateGameState() {
	// clear current frame
	glClear(GL_COLOR_BUFFER_BIT);
	axes->updatePhysicalState();
	axes->render();
	pg->updatePhysicalState();
	pg->render();
	pendulum->updatePhysicalState();
	pendulum->render();
	rope->updatePhysicalState();
	rope->render();
}

RocketSlingersController::~RocketSlingersController() {
	delete pg;
	delete pendulum;
	delete axes;
	delete gameState;
	delete rope;

	std::cout << "controller destructed" << std::endl;
}
