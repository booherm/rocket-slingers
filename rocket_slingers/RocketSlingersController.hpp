#ifndef ROCKETSLINGERSCONTROLLER_HPP
#define ROCKETSLINGERSCONTROLLER_HPP

#include "GameState.hpp"

class RocketSlingersController {
public:
	RocketSlingersController();
	void start();
	~RocketSlingersController();

private:
	void initSdl();
	GameState* gameState;
};

#endif
