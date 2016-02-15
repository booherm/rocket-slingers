#include "Stage.hpp"

Stage::Stage(GameState* gameState) {
	this->gameState = gameState;
	stageObjects.push_back(new PoBackground(gameState));
	stageObjects.push_back(new PoAxes(gameState));
	stageObjects.push_back(new PoGuy(gameState));
	stageObjects.push_back(new PoPendulum(gameState));
	stageObjects.push_back(new PoRope(gameState));
}

void Stage::update() {
	
	for (auto &obj : stageObjects) {
		obj->updatePhysicalState();
		obj->updateRenderState();
	}
}

Stage::~Stage() {

	for (auto &obj : stageObjects) {
		delete obj;
	}
}
