#ifndef STAGE_HPP
#define STAGE_HPP

#include "PoBackground.hpp"
#include "PoAxes.hpp"
#include "PoGuy.hpp"
#include "PoPendulum.hpp"
#include "PoRope.hpp"
//#include "GameState.hpp"

class GameState;
//class PhysicalObject;

class Stage
{
public:

	Stage(GameState* gameState);
	void update();
	~Stage();

private:
	
	GameState* gameState;
	std::vector<PhysicalObject*> stageObjects;
};

#endif
