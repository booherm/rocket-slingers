#include "Stage.hpp"

Stage::Stage(GameState* gameState) {
	this->gameState = gameState;

	//gameState->audioManager->playMusicTrack(AudioManager::MusicTrackId::DRIPPY_EYE);

	stageObjects.push_back(new PoBackground(gameState));
	stageObjects.push_back(new PoAxes(gameState));
	stageObjects.push_back(new PoGuy(gameState));
	stageObjects.push_back(new PoPendulum(gameState));
	//stageObjects.push_back(new PoPendulum2(gameState));
	//stageObjects.push_back(new PoRope(gameState));
	//stageObjects.push_back(new PoBetterRope(gameState));
	stageObjects.push_back(new PoRigidBodyRope(gameState));
	stageObjects.push_back(new PoReticule(gameState));

	//stageObjects.push_back(new PoPhysicsRenderer(gameState));

}

void Stage::update() {
	
	for (auto &obj : stageObjects) {
		obj->updatePhysicalState();
	}

	gameState->physicsManager->updatePhysics();

	for (auto &obj : stageObjects) {
		obj->updateRenderState();
	}
}

Stage::~Stage() {

	for (auto &obj : stageObjects) {
		delete obj;
	}
}
