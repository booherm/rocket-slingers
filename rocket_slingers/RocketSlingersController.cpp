#include "RocketSlingersController.hpp"

RocketSlingersController::RocketSlingersController() {
	renderWindow = new RenderWindow();
}

RocketSlingersController::~RocketSlingersController() {
	delete renderWindow;
}
