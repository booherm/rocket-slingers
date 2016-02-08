#ifndef ROCKETSLINGERSCONTROLLER_HPP
#define ROCKETSLINGERSCONTROLLER_HPP

#include "GameState.hpp"
#include "PoAxes.hpp"
#include "PoPendulum.hpp"
#include "PoGuy.hpp"
#include "PoRope.hpp"

class RocketSlingersController {
public:
	RocketSlingersController();
	void start();
	~RocketSlingersController();

private:
	void initGameObjects();
	void updateGameState();
	GameState* gameState;
	PhysicalObject* pg;
	PhysicalObject* pendulum;
	PhysicalObject* axes;
	PhysicalObject* rope;
};

#endif
