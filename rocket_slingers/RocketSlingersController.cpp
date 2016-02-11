#include "RocketSlingersController.hpp"
#include <iostream>

RocketSlingersController::RocketSlingersController() {
	gameState = new GameState();
	initGameObjects();
	std::cout << "controller constructed" << std::endl;
}

void RocketSlingersController::initGameObjects() {
	poAxes = new PoAxes(gameState);
	poGuy = new PoGuy(gameState);
	poPendulum = new PoPendulum(gameState);
	//rope = new PoRope(gameState);
}

void RocketSlingersController::start() {
	std::cout << "controller start" << std::endl;
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
	// clear current frame
	poAxes->updatePhysicalState();
	poGuy->updatePhysicalState();
	poPendulum->updatePhysicalState();
	//rope->updatePhysicalState();

}

RocketSlingersController::~RocketSlingersController() {
	delete poAxes;
	delete poGuy;
	delete poPendulum;
	delete gameState;
//	delete rope;

	std::cout << "controller destructed" << std::endl;
}
