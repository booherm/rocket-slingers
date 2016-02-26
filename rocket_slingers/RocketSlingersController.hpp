#ifndef ROCKETSLINGERSCONTROLLER_HPP
#define ROCKETSLINGERSCONTROLLER_HPP

#include "GameState.hpp"
#include <windows.h>

class RocketSlingersController {
public:

	RocketSlingersController();
	void start();
	~RocketSlingersController();

private:

	GameState* gameState;
	FILTERKEYS userFilterKeySettings;

	void initSdl();
	void disableKeyRepeat();
	void enableKeyRepeat();

};

#endif
