#ifndef ROCKETSLINGERSCONTROLLER_HPP
#define ROCKETSLINGERSCONTROLLER_HPP

#include "GameState.hpp"
#include "PoAxes.hpp"
#include "PoGuy.hpp"
#include "PoPendulum.hpp"
#include "PoRope.hpp"

class RocketSlingersController {
public:
	RocketSlingersController();
	void start();
	~RocketSlingersController();

private:
	void initSdl();
	void initGameObjects();
	void updateGameState();
	GameState* gameState;
	PhysicalObject* poAxes;
	PhysicalObject* poGuy;
	PhysicalObject* poPendulum;
	PhysicalObject* poRope;
};

#endif
