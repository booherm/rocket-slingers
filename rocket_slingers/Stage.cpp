#include "Stage.hpp"

Stage::Stage(GameState* gameState) {
	this->gameState = gameState;

	//gameState->audioManager->playMusicTrack(AudioManager::MusicTrackId::DRIPPY_EYE);
	//gameState->audioManager->playMusicTrack(AudioManager::MusicTrackId::MOTORLICKER);

	stageObjects.push_back(new PoBackground(gameState));
	stageObjects.push_back(new PoAxes(gameState));
	stageObjects.push_back(new PoGuy(gameState));
	stageObjects.push_back(new PoPendulum(gameState));
	//stageObjects.push_back(new PoPendulum2(gameState));
	//stageObjects.push_back(new PoRope(gameState));
	//stageObjects.push_back(new PoBetterRope(gameState));
	stageObjects.push_back(new PoRigidBodyRope(gameState));
	stageObjects.push_back(new PoReticule(gameState));

	PoTextLabel* alphaLabel = new PoTextLabel(gameState);
	alphaLabel->setTextValue("Rocket Slingers Alpha v" + gameState->version);
	alphaLabel->setColor(glm::vec4(0.0f, 1.0f, 0.0f, 0.25f));
	alphaLabel->setWorldPosition(glm::vec3(0.5f, 0.5f, 0.0f));
	stageObjects.push_back(alphaLabel);

	PoTextLabel* fpsLabel = new PoTextLabel(gameState);
	fpsLabel->setTextValue("FPS: ");
	fpsLabel->setColor(glm::vec4(0.0f, 1.0f, 0.0f, 0.25f));
	fpsLabel->setWorldPosition(glm::vec3((gameState->worldViewportScaler * gameState->aspectRatio) - 3.0f, 0.5f, 0.0f));
	stageObjects.push_back(fpsLabel);


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
