#include "Stage.hpp"

Stage::Stage(GameState* gameState) {
	this->gameState = gameState;

	//gameState->audioManager->playMusicTrack(AudioManager::MusicTrackId::DRIPPY_EYE);
	//gameState->audioManager->playMusicTrack(AudioManager::MusicTrackId::MOTORLICKER);

	stageObjects.push_back(new PoPhysicsRenderer("PHYSICS_RENDERER", gameState));
	//stageObjects.push_back(new PoBackground("BACKGROUND", gameState));
	stageObjects.push_back(new PoGuy("GUY", gameState));
	stageObjects.push_back(new PoPendulum("PENDULUM", gameState));
	stageObjects.push_back(new PoRope("ROPE", gameState));
	stageObjects.push_back(new PoReticule("RETICULE", gameState));
	stageObjects.push_back(new PoAxes("AXES", gameState));

	PoTextLabel* alphaLabel = new PoTextLabel("ALPHA_LABEL", gameState);
	alphaLabel->setTextValue("Rocket Slingers Alpha v" + gameState->version);
	alphaLabel->setColor(glm::vec4(0.0f, 1.0f, 0.0f, 0.25f));
	alphaLabel->setWorldPosition(glm::vec3(0.5f, 0.5f, 0.0f));
	stageObjects.push_back(alphaLabel);

	PoTextLabelFps* fpsLabel = new PoTextLabelFps("FPS_LABEL", gameState);
	fpsLabel->setColor(glm::vec4(0.0f, 1.0f, 0.0f, 0.25f));
	fpsLabel->setWorldPosition(glm::vec3((gameState->worldViewportScaler * gameState->aspectRatio) - 3.0f, 0.5f, 0.0f));
	stageObjects.push_back(fpsLabel);

}

void Stage::update() {
	
	Event fpsUpdateEvent;
	fpsUpdateEvent.eventType = Event::EventType::GAME_EVENT;
	fpsUpdateEvent.gameEvent = Event::GameEvent::FPS_LABEL_UDPATE;
	gameState->eventBus->postEvent(fpsUpdateEvent);

	for (auto &obj : stageObjects) {
		obj->updatePhysicalState();
	}

	gameState->physicsManager->updatePhysics();

}

Stage::~Stage() {

	for (auto &obj : stageObjects) {
		delete obj;
	}
}
