#include "Stage.hpp"

Stage::Stage(GameState* gameState) {
	this->gameState = gameState;

	//gameState->audioManager->playMusicTrack(AudioManager::MusicTrackId::DRIPPY_EYE);
	//gameState->audioManager->playMusicTrack(AudioManager::MusicTrackId::MOTORLICKER);
	gameState->audioManager->setSoundEffectsVolume(0);
	
	pushStageObject(new PoGuy("GUY", gameState), 3);
	pushStageObject(new PoBackground("BACKGROUND", gameState), 1);
	pushStageObject(new PoRopeTarget("ROPE_TARGET", gameState), 2);
	pushStageObject(new PoPendulum("PENDULUM", gameState), 2);
	pushStageObject(new PoRope("ROPE", gameState), 2);
	//pushStageObject(new PoReticule("RETICULE", gameState), 2);
	pushStageObject(new PoAxes("AXES", gameState), 2);
	//pushStageObject(new PoBox("BOX", gameState), 2);
	pushStageObject(new PoPhysicsRenderer("PHYSICS_RENDERER", gameState), 2);

	PoBoundary* floorBoundary = new PoBoundary("FLOOR", gameState, glm::vec3(100.0f, 5.0f, 0.0f), glm::vec3(0.0f, -5.0f, 0.0f));
	pushStageObject(floorBoundary, 1);

	PoBoundary* leftBoundary = new PoBoundary("LEFT_WALL", gameState, glm::vec3(5.0f, 100.0f, 0.0f), glm::vec3(-5.0f, 0.0f, 0.0f));
	pushStageObject(leftBoundary, 1);

	PoBoundary* rightBoundary = new PoBoundary("RIGHT_WALL", gameState, glm::vec3(5.0f, 100.0f, 0.0f), glm::vec3(100.0f, 0.0f, 0.0f));
	pushStageObject(rightBoundary, 1);

	PoBoundary* ceilingBoundary = new PoBoundary("CEILING", gameState, glm::vec3(100.0f, 5.0f, 0.0f), glm::vec3(0.0f, 100.0f, 0.0f));
	pushStageObject(ceilingBoundary, 1);

	PoTextLabel* alphaLabel = new PoTextLabel("ALPHA_LABEL", gameState);
	alphaLabel->setTextValue("Rocket Slingers Alpha v" + gameState->version);
	alphaLabel->setColor(glm::vec4(0.0f, 1.0f, 0.0f, 0.25f));
	alphaLabel->setWorldPosition(glm::vec3(0.5f, 0.5f, 0.0f));
	pushStageObject(alphaLabel, 2);

	PoTextLabelFps* fpsLabel = new PoTextLabelFps("FPS_LABEL", gameState);
	fpsLabel->setColor(glm::vec4(0.0f, 1.0f, 0.0f, 0.25f));
	fpsLabel->setWorldPosition(glm::vec3((gameState->worldViewportScaler * gameState->aspectRatio) - 3.0f, 0.5f, 0.0f));
	pushStageObject(fpsLabel, 2);

}

void Stage::update() {
	Event fpsUpdateEvent;
	fpsUpdateEvent.eventType = Event::EventType::GAME_EVENT;
	fpsUpdateEvent.gameEvent = Event::GameEvent::FPS_LABEL_UDPATE;
	gameState->eventBus->postEvent(fpsUpdateEvent);

	for (auto obj = stageObjectsInUpdateOrder.begin(); obj != stageObjectsInUpdateOrder.end(); ++obj) {
		obj->second->updatePhysicalState();
	}

	gameState->physicsManager->updatePhysics();
}

void Stage::pushStageObject(PhysicalObject* physicalObject, unsigned int objectUpdateOrder) {

	StageObjectKeyUpdateOrder updateOrderKey(objectUpdateOrder, physicalObject->objectId);
	stageObjectsInUpdateOrder[updateOrderKey] = physicalObject;
	stageObjectsById[physicalObject->objectId] = physicalObject;
}

Stage::~Stage() {

	for (auto obj = stageObjectsInUpdateOrder.begin(); obj != stageObjectsInUpdateOrder.end(); ++obj) {
		delete obj->second;
	}
}

PhysicalObject* Stage::getPhysicalObject(const std::string& objectId) {
	return stageObjectsById[objectId];
}
