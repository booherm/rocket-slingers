#include "RocketSlingersController.hpp"
#include <iostream>
#include "Stage.hpp"
#pragma comment(lib, "user32.lib")    

RocketSlingersController::RocketSlingersController() {

	disableKeyRepeat();
	initSdl();
	gameState = new GameState();

}

void RocketSlingersController::initSdl() {

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		std::string sdlError = SDL_GetError();
		throw "RocketSlingersController::initSdl - SDL initialization failed: " + sdlError;
	}
	SDL_DisableScreenSaver();
	SDL_SetRelativeMouseMode(SDL_TRUE);
}

void RocketSlingersController::disableKeyRepeat() {

	// get and store user filter key settings
	userFilterKeySettings.cbSize = sizeof(FILTERKEYS);
	int getFilterKeySettingsResult = SystemParametersInfo(SPI_GETFILTERKEYS, 0, &userFilterKeySettings, userFilterKeySettings.cbSize);

	// modify filter key settings to disable key repeat
	FILTERKEYS modifiedFilterKeySettings = userFilterKeySettings;
	modifiedFilterKeySettings.cbSize = sizeof(FILTERKEYS);
	modifiedFilterKeySettings.dwFlags = modifiedFilterKeySettings.dwFlags | 1;
	modifiedFilterKeySettings.iWaitMSec = 0;
	modifiedFilterKeySettings.iDelayMSec = 0;
	modifiedFilterKeySettings.iRepeatMSec = 0;
	modifiedFilterKeySettings.iBounceMSec = 0;
	int setFilterKeySettingsResult = SystemParametersInfo(SPI_SETFILTERKEYS, 0, &modifiedFilterKeySettings, modifiedFilterKeySettings.cbSize);

}

void RocketSlingersController::enableKeyRepeat() {
	// Turn filter keys off during debugging.  If a crash occurs, it will be left in a filter keys on state.  Running the app
	// again with a clean shutdown will restore it to off.
	userFilterKeySettings.dwFlags = userFilterKeySettings.dwFlags & 0;

	// restore user filter key settings
	int setFilterKeySettingsResult = SystemParametersInfo(SPI_SETFILTERKEYS, 0, &userFilterKeySettings, userFilterKeySettings.cbSize);
}

void RocketSlingersController::start() {
	bool running = true;
	while (running) {
		gameState->frameStart();
		running = gameState->eventBus->processInput();
		gameState->activeStage->update();
		gameState->masterRenderer->renderDefaultFrameBuffer();
		gameState->renderWindow->publishFrame();
		gameState->frameEnd();
	}
}

RocketSlingersController::~RocketSlingersController() {
	delete gameState;
	SDL_Quit();
	enableKeyRepeat();
}
