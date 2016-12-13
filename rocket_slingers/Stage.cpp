#include "Stage.hpp"

Stage::Stage(GameState* gameState) {
	this->gameState = gameState;
}

void Stage::initialize() {

	//gameState->audioManager->playMusicTrack(AudioManager::MusicTrackId::DRIPPY_EYE);
	//gameState->audioManager->playMusicTrack(AudioManager::MusicTrackId::MOTORLICKER);
	//gameState->audioManager->playMusicTrack(AudioManager::MusicTrackId::CLUB_DIVER);
	//gameState->audioManager->playMusicTrack(AudioManager::MusicTrackId::EXIT_THE_PREMISES);
	gameState->audioManager->playMusicTrack(AudioManager::MusicTrackId::FURIOUS_FREAK);

	//gameState->audioManager->setSoundEffectsVolume(0);
	
	pushStageObject(new PoGuy("GUY", gameState), 3);
	pushStageObject(new PoBackground("BACKGROUND", gameState), 1);
	//pushStageObject(new PoShaderTester("SHADER_TESTER", gameState), 1);
	pushStageObject(new PoRopeTarget("ROPE_TARGET", gameState), 2);
	//pushStageObject(new PoPendulum("PENDULUM", gameState), 2);
	pushStageObject(new PoRope("ROPE", gameState), 2);
	pushStageObject(new PoReticule("RETICULE", gameState), 2);
	//pushStageObject(new PoAxes("AXES", gameState), 2);
	//pushStageObject(new PoBox("BOX", gameState), 2);
	//pushStageObject(new PoPhysicsRenderer("PHYSICS_RENDERER", gameState), 4);

	/*
	PoBoundary2* floorBoundary = new PoBoundary2("FLOOR", gameState, glm::vec3(100.0f, 5.0f, 0.0f), glm::vec3(0.0f, -5.0f, 0.0f));
	pushStageObject(floorBoundary, 1);

	PoBoundary2* leftBoundary = new PoBoundary2("LEFT_WALL", gameState, glm::vec3(5.0f, 100.0f, 0.0f), glm::vec3(-5.0f, 0.0f, 0.0f));
	pushStageObject(leftBoundary, 1);

	PoBoundary2* rightBoundary = new PoBoundary2("RIGHT_WALL", gameState, glm::vec3(5.0f, 100.0f, 0.0f), glm::vec3(100.0f, 0.0f, 0.0f));
	pushStageObject(rightBoundary, 1);

	PoBoundary2* ceilingBoundary = new PoBoundary2("CEILING", gameState, glm::vec3(100.0f, 5.0f, 0.0f), glm::vec3(0.0f, 100.0f, 0.0f));
	pushStageObject(ceilingBoundary, 1);
	*/

	PoTubeSection::TileType tubeMaze[10][10];
	tubeMaze[0][0] = PoTubeSection::TileType::STRAIGHT_HORZ;
	tubeMaze[0][1] = PoTubeSection::TileType::STRAIGHT_HORZ;
	tubeMaze[0][2] = PoTubeSection::TileType::STRAIGHT_HORZ;
	tubeMaze[0][3] = PoTubeSection::TileType::STRAIGHT_HORZ;
	tubeMaze[0][4] = PoTubeSection::TileType::STRAIGHT_HORZ;
	tubeMaze[0][5] = PoTubeSection::TileType::STRAIGHT_HORZ;
	tubeMaze[0][6] = PoTubeSection::TileType::STRAIGHT_HORZ;
	tubeMaze[0][7] = PoTubeSection::TileType::STRAIGHT_HORZ;
	tubeMaze[0][8] = PoTubeSection::TileType::STRAIGHT_HORZ;
	tubeMaze[0][9] = PoTubeSection::TileType::LEFT_TOP_90;

	tubeMaze[1][0] = PoTubeSection::TileType::RIGHT_TOP_90;
	tubeMaze[1][1] = PoTubeSection::TileType::STRAIGHT_HORZ;
	tubeMaze[1][2] = PoTubeSection::TileType::STRAIGHT_HORZ;
	tubeMaze[1][3] = PoTubeSection::TileType::STRAIGHT_HORZ;
	tubeMaze[1][4] = PoTubeSection::TileType::STRAIGHT_HORZ;
	tubeMaze[1][5] = PoTubeSection::TileType::STRAIGHT_HORZ;
	tubeMaze[1][6] = PoTubeSection::TileType::STRAIGHT_HORZ;
	tubeMaze[1][7] = PoTubeSection::TileType::LEFT_TOP_90;
	tubeMaze[1][8] = PoTubeSection::TileType::TOP_RIGHT_90;
	tubeMaze[1][9] = PoTubeSection::TileType::LEFT_BOTTOM_90;

	tubeMaze[2][0] = PoTubeSection::TileType::RIGHT_BOTTOM_90;
	tubeMaze[2][1] = PoTubeSection::TileType::STRAIGHT_HORZ;
	tubeMaze[2][2] = PoTubeSection::TileType::STRAIGHT_HORZ;
	tubeMaze[2][3] = PoTubeSection::TileType::STRAIGHT_HORZ;
	tubeMaze[2][4] = PoTubeSection::TileType::LEFT_TOP_90;
	tubeMaze[2][5] = PoTubeSection::TileType::TOP_RIGHT_90;
	tubeMaze[2][6] = PoTubeSection::TileType::LEFT_TOP_90;
	tubeMaze[2][7] = PoTubeSection::TileType::STRAIGHT_VERT;
	tubeMaze[2][8] = PoTubeSection::TileType::RIGHT_BOTTOM_90;
	tubeMaze[2][9] = PoTubeSection::TileType::LEFT_TOP_90;

	tubeMaze[3][0] = PoTubeSection::TileType::TOP_RIGHT_90;
	tubeMaze[3][1] = PoTubeSection::TileType::STRAIGHT_HORZ;
	tubeMaze[3][2] = PoTubeSection::TileType::STRAIGHT_HORZ;
	tubeMaze[3][3] = PoTubeSection::TileType::LEFT_TOP_90;
	tubeMaze[3][4] = PoTubeSection::TileType::STRAIGHT_VERT;
	tubeMaze[3][5] = PoTubeSection::TileType::STRAIGHT_VERT;
	tubeMaze[3][6] = PoTubeSection::TileType::RIGHT_BOTTOM_90;
	tubeMaze[3][7] = PoTubeSection::TileType::LEFT_BOTTOM_90;
	tubeMaze[3][8] = PoTubeSection::TileType::TOP_RIGHT_90;
	tubeMaze[3][9] = PoTubeSection::TileType::LEFT_BOTTOM_90;

	tubeMaze[4][0] = PoTubeSection::TileType::RIGHT_BOTTOM_90;
	tubeMaze[4][1] = PoTubeSection::TileType::STRAIGHT_HORZ;
	tubeMaze[4][2] = PoTubeSection::TileType::LEFT_TOP_90;
	tubeMaze[4][3] = PoTubeSection::TileType::STRAIGHT_VERT;
	tubeMaze[4][4] = PoTubeSection::TileType::STRAIGHT_VERT;
	tubeMaze[4][5] = PoTubeSection::TileType::STRAIGHT_VERT;
	tubeMaze[4][6] = PoTubeSection::TileType::TOP_RIGHT_90;
	tubeMaze[4][7] = PoTubeSection::TileType::LEFT_TOP_90;
	tubeMaze[4][8] = PoTubeSection::TileType::RIGHT_BOTTOM_90;
	tubeMaze[4][9] = PoTubeSection::TileType::LEFT_TOP_90;

	tubeMaze[5][0] = PoTubeSection::TileType::STRAIGHT_HORZ;
	tubeMaze[5][1] = PoTubeSection::TileType::LEFT_TOP_90;
	tubeMaze[5][2] = PoTubeSection::TileType::STRAIGHT_VERT;
	tubeMaze[5][3] = PoTubeSection::TileType::RIGHT_BOTTOM_90;
	tubeMaze[5][4] = PoTubeSection::TileType::LEFT_BOTTOM_90;
	tubeMaze[5][5] = PoTubeSection::TileType::RIGHT_BOTTOM_90;
	tubeMaze[5][6] = PoTubeSection::TileType::LEFT_BOTTOM_90;
	tubeMaze[5][7] = PoTubeSection::TileType::STRAIGHT_VERT;
	tubeMaze[5][8] = PoTubeSection::TileType::TOP_RIGHT_90;
	tubeMaze[5][9] = PoTubeSection::TileType::LEFT_BOTTOM_90;

	tubeMaze[6][0] = PoTubeSection::TileType::TOP_RIGHT_90;
	tubeMaze[6][1] = PoTubeSection::TileType::LEFT_BOTTOM_90;
	tubeMaze[6][2] = PoTubeSection::TileType::RIGHT_BOTTOM_90;
	tubeMaze[6][3] = PoTubeSection::TileType::STRAIGHT_HORZ;
	tubeMaze[6][4] = PoTubeSection::TileType::STRAIGHT_HORZ;
	tubeMaze[6][5] = PoTubeSection::TileType::LEFT_TOP_90;
	tubeMaze[6][6] = PoTubeSection::TileType::TOP_RIGHT_90;
	tubeMaze[6][7] = PoTubeSection::TileType::LEFT_BOTTOM_90;
	tubeMaze[6][8] = PoTubeSection::TileType::RIGHT_BOTTOM_90;
	tubeMaze[6][9] = PoTubeSection::TileType::LEFT_TOP_90;

	tubeMaze[7][0] = PoTubeSection::TileType::STRAIGHT_VERT;
	tubeMaze[7][1] = PoTubeSection::TileType::TOP_RIGHT_90;
	tubeMaze[7][2] = PoTubeSection::TileType::STRAIGHT_HORZ;
	tubeMaze[7][3] = PoTubeSection::TileType::STRAIGHT_HORZ;
	tubeMaze[7][4] = PoTubeSection::TileType::LEFT_TOP_90;
	tubeMaze[7][5] = PoTubeSection::TileType::STRAIGHT_VERT;
	tubeMaze[7][6] = PoTubeSection::TileType::STRAIGHT_VERT;
	tubeMaze[7][7] = PoTubeSection::TileType::TOP_RIGHT_90;
	tubeMaze[7][8] = PoTubeSection::TileType::STRAIGHT_HORZ;
	tubeMaze[7][9] = PoTubeSection::TileType::LEFT_BOTTOM_90;

	tubeMaze[8][0] = PoTubeSection::TileType::STRAIGHT_VERT;
	tubeMaze[8][1] = PoTubeSection::TileType::RIGHT_BOTTOM_90;
	tubeMaze[8][2] = PoTubeSection::TileType::STRAIGHT_HORZ;
	tubeMaze[8][3] = PoTubeSection::TileType::LEFT_TOP_90;
	tubeMaze[8][4] = PoTubeSection::TileType::STRAIGHT_VERT;
	tubeMaze[8][5] = PoTubeSection::TileType::STRAIGHT_VERT;
	tubeMaze[8][6] = PoTubeSection::TileType::STRAIGHT_VERT;
	tubeMaze[8][7] = PoTubeSection::TileType::RIGHT_BOTTOM_90;
	tubeMaze[8][8] = PoTubeSection::TileType::STRAIGHT_HORZ;
	tubeMaze[8][9] = PoTubeSection::TileType::LEFT_TOP_90;

	tubeMaze[9][0] = PoTubeSection::TileType::RIGHT_BOTTOM_90;
	tubeMaze[9][1] = PoTubeSection::TileType::STRAIGHT_HORZ;
	tubeMaze[9][2] = PoTubeSection::TileType::STRAIGHT_HORZ;
	tubeMaze[9][3] = PoTubeSection::TileType::LEFT_BOTTOM_90;
	tubeMaze[9][4] = PoTubeSection::TileType::RIGHT_BOTTOM_90;
	tubeMaze[9][5] = PoTubeSection::TileType::LEFT_BOTTOM_90;
	tubeMaze[9][6] = PoTubeSection::TileType::RIGHT_BOTTOM_90;
	tubeMaze[9][7] = PoTubeSection::TileType::STRAIGHT_HORZ;
	tubeMaze[9][8] = PoTubeSection::TileType::STRAIGHT_HORZ;
	tubeMaze[9][9] = PoTubeSection::TileType::LEFT_BOTTOM_90;

	float scale = 12.0f;
	for (unsigned int r = 0; r < 10; ++r) {
		for (unsigned int c = 0; c < 10; ++c) {
			pushStageObject(
				new PoTubeSection("TUBE_SECTION_" + std::to_string(r) + "_" + std::to_string(c),
					gameState,
					tubeMaze[r][c],
					b2Vec2(scale, scale),
					b2Vec2(30.0f + (scale * c), 10.0f + (scale * r))
				),
				3
			);

		}
	}


	//pushStageObject(new PoTubeSection("TUBE_SECTION_1", gameState, PoTubeSection::TileType::STRAIGHT_HORZ, b2Vec2(20.0f, 20.0f), b2Vec2(30.0f, 10.0f)), 3);
	//pushStageObject(new PoTubeSection("TUBE_SECTION_1", gameState, PoTubeSection::TileType::STRAIGHT_VERT, b2Vec2(20.0f, 20.0f), b2Vec2(30.0f, 10.0f)), 3);
	//pushStageObject(new PoTubeSection("TUBE_SECTION_2", gameState, PoTubeSection::TileType::LEFT_BOTTOM_90, b2Vec2(20.0f, 20.0f), b2Vec2(50.0f, 10.0f)), 3);
	//pushStageObject(new PoTubeSection("TUBE_SECTION_2", gameState, PoTubeSection::TileType::LEFT_TOP_90, b2Vec2(20.0f, 20.0f), b2Vec2(50.0f, 10.0f)), 3);
	//pushStageObject(new PoTubeSection("TUBE_SECTION_2", gameState, PoTubeSection::TileType::TOP_LEFT_90, b2Vec2(20.0f, 20.0f), b2Vec2(50.0f, 10.0f)), 3);
	//pushStageObject(new PoTubeSection("TUBE_SECTION_2", gameState, PoTubeSection::TileType::TOP_RIGHT_90, b2Vec2(20.0f, 20.0f), b2Vec2(50.0f, 10.0f)), 3);
	//pushStageObject(new PoTubeSection("TUBE_SECTION_2", gameState, PoTubeSection::TileType::RIGHT_TOP_90, b2Vec2(20.0f, 20.0f), b2Vec2(50.0f, 10.0f)), 3);
	//pushStageObject(new PoTubeSection("TUBE_SECTION_2", gameState, PoTubeSection::TileType::RIGHT_BOTTOM_90, b2Vec2(20.0f, 20.0f), b2Vec2(50.0f, 10.0f)), 3);
	//pushStageObject(new PoTubeSection("TUBE_SECTION_2", gameState, PoTubeSection::TileType::BOTTOM_LEFT_90, b2Vec2(20.0f, 20.0f), b2Vec2(50.0f, 10.0f)), 3);
	//pushStageObject(new PoTubeSection("TUBE_SECTION_2", gameState, PoTubeSection::TileType::BOTTOM_RIGHT_90, b2Vec2(20.0f, 20.0f), b2Vec2(50.0f, 10.0f)), 3);


/*
	pushStageObject(new PoSpinAnchor("SPIN_ANCHOR_1", gameState, glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(10.0f, 10.0f, 0.0f)), 3);
	pushStageObject(new PoSpinAnchor("SPIN_ANCHOR_2", gameState, glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(20.0f, 20.0f, 0.0f)), 3);
	pushStageObject(new PoSpinAnchor("SPIN_ANCHOR_3", gameState, glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(10.0f, 30.0f, 0.0f)), 3);
	pushStageObject(new PoSpinAnchor("SPIN_ANCHOR_4", gameState, glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(20.0f, 40.0f, 0.0f)), 3);
	*/

	pushStageObject(new PoBall("BALL_1", gameState, 2.0f, b2Vec2(5.0f, 20.0f)), 3);
	//pushStageObject(new PoBall("BALL_2", gameState, 3.0f, b2Vec2(25.0f, 20.0f)), 3);


	PoTextLabel* alphaLabel = new PoTextLabel("ALPHA_LABEL", gameState);
	alphaLabel->setTextValue("Rocket Slingers Alpha v" + gameState->version);
	alphaLabel->setColor(glm::vec4(0.0f, 1.0f, 0.0f, 0.25f));
	alphaLabel->setWorldPosition(glm::vec3(0.5f, 0.5f, 0.0f));
	pushStageObject(alphaLabel, 2);

	PoTextLabelFps* fpsLabel = new PoTextLabelFps("FPS_LABEL", gameState);
	fpsLabel->setColor(glm::vec4(0.0f, 1.0f, 0.0f, 0.25f));
	fpsLabel->setWorldPosition(glm::vec3((gameState->worldViewportWidth) - 3.0f, 0.5f, 0.0f));
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
