#ifndef STAGE_HPP
#define STAGE_HPP

#include "PoBackground.hpp"
#include "PoAxes.hpp"
#include "PoGuy.hpp"
#include "PoPendulum.hpp"
#include "PoRigidBodyRope.hpp"
#include "PoPhysicsRenderer.hpp"
#include "PoReticule.hpp"
#include "PoTextLabel.hpp"
#include "PoTextLabelFps.hpp"

class GameState;

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
