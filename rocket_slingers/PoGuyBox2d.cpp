#include "PoGuyBox2d.hpp"

PoGuyBox2d::PoGuyBox2d(const std::string& objectId, GameState* gameState) : PhysicalObject(objectId, gameState) {
	initShaders();
	initGeometry();
	initPhysics();
	initEventSubsriptions();

	rocketImpulseMagnitude = 10.0f;
	keyDownCount = 0;
	keyStates[SDLK_w] = false;
	keyStates[SDLK_s] = false;
	keyStates[SDLK_a] = false;
	keyStates[SDLK_d] = false;
	rocketOn = false;
	ropeAttachedToStructure = false;
	gravityOn = true;
}

void PoGuyBox2d::initEventSubsriptions() {
	gameState->eventBus->subscribeToKeyboardEvent(SDL_PRESSED, SDLK_w, this);
	gameState->eventBus->subscribeToKeyboardEvent(SDL_PRESSED, SDLK_s, this);
	gameState->eventBus->subscribeToKeyboardEvent(SDL_PRESSED, SDLK_a, this);
	gameState->eventBus->subscribeToKeyboardEvent(SDL_PRESSED, SDLK_d, this);
	gameState->eventBus->subscribeToKeyboardEvent(SDL_RELEASED, SDLK_w, this);
	gameState->eventBus->subscribeToKeyboardEvent(SDL_RELEASED, SDLK_s, this);
	gameState->eventBus->subscribeToKeyboardEvent(SDL_RELEASED, SDLK_a, this);
	gameState->eventBus->subscribeToKeyboardEvent(SDL_RELEASED, SDLK_d, this);
	gameState->eventBus->subscribeToKeyboardEvent(SDL_PRESSED, SDLK_g, this);
	gameState->eventBus->subscribeToMouseButtonEvent(SDL_PRESSED, SDL_BUTTON_LEFT, this);
	gameState->eventBus->subscribeToMouseButtonEvent(SDL_PRESSED, SDL_BUTTON_RIGHT, this);
}

void PoGuyBox2d::sdlInputEventCallback(const Event& eventObj) {

	if (eventObj.eventType == Event::EventType::SDL_MOUSE_BUTTON) {

		if (eventObj.sdlInputEvent->button.button == SDL_BUTTON_LEFT) {

			// throw rope
			Event deployRopeEvent;
			deployRopeEvent.eventPoster = this;
			deployRopeEvent.eventType = Event::EventType::GAME_EVENT;
			deployRopeEvent.gameEvent = Event::GameEvent::THROW_ROPE;
			deployRopeEvent.eventWorldCoordinateX = eventObj.eventWorldCoordinateX;
			deployRopeEvent.eventWorldCoordinateY = eventObj.eventWorldCoordinateY;
			gameState->eventBus->postEvent(deployRopeEvent);

		}
		else {

			// right mouse button - release rope
			Event releaseRopeEvent;
			releaseRopeEvent.eventPoster = this;
			releaseRopeEvent.eventType = Event::EventType::GAME_EVENT;
			releaseRopeEvent.gameEvent = Event::GameEvent::RELEASE_ROPE;
			releaseRopeEvent.eventWorldCoordinateX = eventObj.eventWorldCoordinateX;
			releaseRopeEvent.eventWorldCoordinateY = eventObj.eventWorldCoordinateY;
			gameState->eventBus->postEvent(releaseRopeEvent);

		}
	}
	else {
		if (eventObj.sdlInputEvent->key.state == SDL_PRESSED) {

			if (eventObj.sdlInputEvent->key.keysym.sym == SDLK_g) {
				if (gravityOn) {
					rigidBody->SetGravityScale(0.0f);
					//physicalMass->setGravity(glm::vec3(0.0f, 0.0f, 0.0f));
					gravityOn = false;
				}
				else
				{
					//physicalMass->setGravity(glm::vec3(0.0f, -9.81f, 0.0f));
					rigidBody->SetGravityScale(1.0f);
					gravityOn = true;
				}
			}

			// log key down state
			keyStates[eventObj.sdlInputEvent->key.keysym.sym] = true;
			keyDownCount++;

			// turn on rocket and play sound effect if it is not alreay on
			if (!rocketOn) {
				rocketOn = true;
				soundEffectInstanceId = gameState->audioManager->playSoundEffect(AudioManager::SoundEffectId::ROCKET_RUMBLE, -1);
			}

		}
		else {

			// log key up state
			keyStates[eventObj.sdlInputEvent->key.keysym.sym] = false;
			keyDownCount--;

			// if no keys are down, stop rocket
			if (keyDownCount == 0) {
				gameState->audioManager->stopSoundEffect(soundEffectInstanceId);
				rocketOn = false;
			}

		}
	}
}

void PoGuyBox2d::initShaders() {

	// vertex shader
	std::string vertexShaderSource =
		"#version 330 core\n"
		"\n"
		"layout (location = 0) in vec3 modelVertex;\n"
		"layout (location = 1) in vec4 colorValue;\n"
		"\n"
		"out vec4 fragColor;\n"
		"\n"
		"uniform vec3 modelOriginOffset;\n"
		"uniform mat4 transformMatrix;\n"
		"\n"
		"void main()\n"
		"{\n"
		"    gl_Position = transformMatrix * vec4(modelVertex + modelOriginOffset, 1.0f);\n"
		"    fragColor = colorValue;\n"
		"}\n";

	// fragment shader
	std::string fragmentShaderSource =
		"#version 330 core\n"
		"\n"
		"in vec4 fragColor;\n"
		"\n"
		"out vec4 color;\n"
		"\n"
		"void main()\n"
		"{\n"
		"    color = fragColor;\n"
		"}\n";

	shaderProg = OglShaderProgram();
	shaderProg.createVertexShaderFromSourceString(vertexShaderSource);
	shaderProg.createFragmentShaderFromSourceString(fragmentShaderSource);
	shaderProg.build();

}

void PoGuyBox2d::initGeometry() {

	// buffer axes model data
	float guyModelVerticesSet[57][2] = {
		{ 0.0f, 0.0f },
		{ 0.0f, 5.0f },
		{ 2.0f, 4.0f },
		{ 2.5f, 5.0f },
		{ 3.0f, 4.0f },
		{ 6.0f, 4.0f },
		{ 7.0f, 8.0f },
		{ 8.0f, 11.0f },
		{ 8.0f, 15.0f },
		{ 7.0f, 16.0f },
		{ 7.0f, 18.0f },
		{ 7.5f, 20.0f },
		{ 11.0f, 28.0f },
		{ 11.5f, 29.0f },
		{ 11.0f, 31.0f },
		{ 10.0f, 33.0f },
		{ 9.0f, 35.5f },
		{ 10.0f, 36.5f },
		{ 15.0f, 40.0f },
		{ 15.5f, 41.5f },
		{ 15.5f, 43.0f },
		{ 16.0f, 45.0f },
		{ 17.0f, 46.0f },
		{ 18.5f, 46.5f },
		{ 20.0f, 46.0f },
		{ 21.0f, 45.0f },
		{ 21.5f, 43.0f },
		{ 21.0f, 40.0f },
		{ 24.0f, 37.0f },
		{ 26.0f, 33.0f },
		{ 25.5f, 31.0f },
		{ 23.5f, 28.5f },
		{ 26.0f, 20.0f },
		{ 28.0f, 8.0f },
		{ 29.5f, 6.0f },
		{ 30.0f, 4.0f },
		{ 33.0f, 4.0f },
		{ 35.0f, 2.5f },
		{ 33.0f, 1.0f },
		{ 3.0f, 1.0f },
		{ 2.5f, 0.0f },
		{ 2.0f, 1.0f },
		{ 10.5f, 4.0f },
		{ 9.5f, 5.0f },
		{ 9.5f, 8.0f },
		{ 11.5f, 12.0f },
		{ 12.0f, 16.0f },
		{ 14.0f, 20.0f },
		{ 16.0f, 22.0f },
		{ 17.0f, 22.0f },
		{ 18.5f, 19.0f },
		{ 19.0f, 16.0f },
		{ 20.0f, 15.0f },
		{ 21.5f, 14.5f },
		{ 22.5f, 13.5f },
		{ 24.0f, 9.0f },
		{ 24.5f, 4.0f }
	};

	unsigned int guyModelTriangleVertexIds[53][3] = {
		{ 0, 41, 1 },
		{ 41, 2, 1 },
		{ 41, 4, 2 },
		{ 2, 4, 3 },
		{ 40, 39, 41 },
		{ 41, 39, 4 },
		{ 39, 36, 4 },
		{ 39, 38, 36 },
		{ 38, 37, 36 },
		{ 5, 42, 43 },
		{ 5, 43, 44 },
		{ 5, 44, 6 },
		{ 6, 44, 7 },
		{ 44, 45, 7 },
		{ 7, 45, 8 },
		{ 45, 46, 8 },
		{ 8, 46, 9 },
		{ 9, 46, 10 },
		{ 46, 47, 10 },
		{ 10, 47, 11 },
		{ 11, 47, 48 },
		{ 11, 48, 12 },
		{ 48, 49, 12 },
		{ 49, 32, 12 },
		{ 32, 31, 12 },
		{ 12, 31, 13 },
		{ 13, 31, 30 },
		{ 13, 30, 14 },
		{ 14, 30, 15 },
		{ 30, 29, 15 },
		{ 15, 29, 16 },
		{ 16, 29, 17 },
		{ 29, 28, 17 },
		{ 17, 28, 18 },
		{ 28, 27, 18 },
		{ 18, 27, 19 },
		{ 27, 26, 19 },
		{ 19, 26, 20 },
		{ 20, 26, 25 },
		{ 20, 25, 21 },
		{ 21, 25, 24 },
		{ 21, 24, 22 },
		{ 22, 24, 23 },
		{ 50, 32, 49 },
		{ 51, 32, 50 },
		{ 52, 32, 51 },
		{ 53, 32, 52 },
		{ 54, 32, 53 },
		{ 33, 32, 54 },
		{ 55, 33, 54 },
		{ 56, 33, 55 },
		{ 56, 34, 33 },
		{ 56, 35, 34 }
	};

	// push triangle vertices
	unsigned int guyModelVertexIndex = 0;
	for (unsigned int t = 0; t < 53; t++) {
		for (unsigned int v = 0; v < 3; v++) {
			modelVertexData.push_back(glm::vec3(guyModelVerticesSet[guyModelTriangleVertexIds[t][v]][0], guyModelVerticesSet[guyModelTriangleVertexIds[t][v]][1], 0.0f));
		}
	}

	// push color data
	for (unsigned int i = 0; i < modelVertexData.size(); i++) {
		colorData.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
	}

	// set model origin
	modelOriginOffset = glm::vec3(-18.0f, -35.0f, 0.0f);

	initModelVertexBuffer();
	initColorBuffer();
	buildVao(MODEL_VERTEX | COLOR);
	gameState->masterRenderer->addRenderableObject(this);
	shouldRender = true;
}

void PoGuyBox2d::initPhysics() {

	initialPosition = glm::vec3(5.0f, 5.0f, 0.0f);
	glm::mat4 worldTransform;
	worldTransform = glm::translate(worldTransform, initialPosition);

	b2BodyDef rigidBodyDef;
	rigidBodyDef.type = b2_dynamicBody;
	rigidBodyDef.position.Set(initialPosition.x, initialPosition.y);
	rigidBody = gameState->physicsManager->box2dWorld->CreateBody(&rigidBodyDef);
	b2CircleShape rigidBodyShape;
	rigidBodyShape.m_radius = 1.0f;
	b2FixtureDef rigidBodyFixtureDef;
	rigidBodyFixtureDef.shape = &rigidBodyShape;
	//rigidBodyFixtureDef.density = 62.0f;
	rigidBodyFixtureDef.density = 1.0f;
	rigidBodyFixtureDef.friction = 0.3f;
	rigidBodyFixtureDef.filter.groupIndex = -1;
	rigidBodyFixtureDef.userData = this;
	rigidBody->CreateFixture(&rigidBodyFixtureDef);

	/*
	glm::mat4 cameraFocalPointTransform;
	cameraFocalPointTransform = glm::translate(cameraFocalPointTransform, glm::vec3(5.0f, 10.0f, 0.0f));


	cameraFocalPointPhysicalMass = new PhysicalMass();
	cameraFocalPointPhysicalMass->init("CAMERA_FOCAL_POINT", gameState, 1.0f, cameraFocalPointTransform, PhysicsManager::CollisionGroup::NO_COLLISION);
	cameraFocalPointPhysicalMass->addToDynamicsWorld();
	cameraFocalPointPhysicalMass->setGravity(glm::vec3());
	cameraFocalPointPhysicalMass->setActivationState(DISABLE_DEACTIVATION);
	*/

	/*

	// 6dof constraint
	btTransform constraintTransform = btTransform::getIdentity();
	cameraSpringConstraint = new btGeneric6DofSpring2Constraint(*(physicalMass->rigidBody), *(cameraFocalPointPhysicalMass->rigidBody), constraintTransform, constraintTransform);
	cameraSpringConstraint->setLimit(0, 10.0f, -100.0f);
	cameraSpringConstraint->setLimit(1, 10.0f, -100.0f);
	cameraSpringConstraint->setLimit(2, 0.0f, 0.0f);
	cameraSpringConstraint->setLimit(3, 0.0f, 0.0f);
	cameraSpringConstraint->setLimit(4, 0.0f, 0.0f);
	cameraSpringConstraint->setLimit(5, 0.0f, 0.0f);
	cameraSpringConstraint->enableSpring(0, true);
	cameraSpringConstraint->setStiffness(0, 50.0f);
	cameraSpringConstraint->setEquilibriumPoint(0, 0);
	cameraSpringConstraint->setDamping(0, 10.0f);

	cameraSpringConstraint->enableSpring(1, true);
	cameraSpringConstraint->setStiffness(1, 50.0f);
	cameraSpringConstraint->setEquilibriumPoint(1, 0);
	cameraSpringConstraint->setDamping(1, 10.0f);

	//cameraSpringConstraint->setBounce(0, 0.0f);
	//cameraSpringConstraint->setLinearLowerLimit(btVector3(-3.0f, 0.0f, 0.0f));
	//cameraSpringConstraint->setLinearUpperLimit(btVector3(3.0f, 0.0f, 0.0f));

	gameState->physicsManager->dynamicsWorld->addConstraint(cameraSpringConstraint);
	*/

	shouldDoPhysicalUpdate = true;



}

void PoGuyBox2d::doPhysicalUpdate() {

	// setup rocket force based on key state
	rocketImpulse = glm::vec3();
	if (rocketOn) {
		if (keyStates[SDLK_d]) {
			rocketImpulse.x += rocketImpulseMagnitude;
		}
		if (keyStates[SDLK_a]) {
			rocketImpulse.x -= rocketImpulseMagnitude;
		}
		if (keyStates[SDLK_w]) {
			rocketImpulse.y += rocketImpulseMagnitude;
		}
		if (keyStates[SDLK_s]) {
			rocketImpulse.y -= rocketImpulseMagnitude;

		}

		// Only apply the rocket impulse to the rigid body if the rope is not attached to a structure.  If it is,
		// the rope will have a mass to represent the player.
		if (!ropeAttachedToStructure) {
			rigidBody->ApplyLinearImpulse(b2Vec2(rocketImpulse.x, rocketImpulse.y), b2Vec2(), true);
		}

	}

	// update camera focal point based on current player position
	//glm::vec3 cameraFocalPointPosition;
	//cameraFocalPointPhysicalMass->getCenterOfMassPosition(cameraFocalPointPosition);

	// debug - this displays a reticule at the camera focal point for debugging
	/*
	Event e;
	e.eventPoster = this;
	e.eventType = Event::EventType::GAME_EVENT;
	e.gameEvent = Event::GameEvent::MOVE_RETICULE;
	e.eventWorldCoordinateX = cameraFocalPointPosition.x;
	e.eventWorldCoordinateY = cameraFocalPointPosition.y;
	gameState->eventBus->postEvent(e);
	*/

	//gameState->camera->updatePosition(glm::vec3(cameraFocalPointPosition.x - 7.5f, cameraFocalPointPosition.y - 7.5f, 0.0f));
	gameState->camera->updatePosition(glm::vec3(rigidBody->GetPosition().x - 7.5f, rigidBody->GetPosition().y - 7.5f, 0.0f));
}

void PoGuyBox2d::getRocketImpulse(glm::vec3& rocketImpulse) {
	// provide rocket force to rope
	rocketImpulse = this->rocketImpulse;
}

/*
void PoGuyBox2d::updateTransformFromRope(const btTransform& transform) {
// updating the position based on rope position
physicalMass->rigidBody->setCenterOfMassTransform(transform);
physicalMass->setCenterOfMassTransform(transform);
}
*/

b2Body* PoGuyBox2d::getRigidBody() {
	return rigidBody;
}

void PoGuyBox2d::render() {

	glm::mat4 modelTransform;  //= rigidBody->GetTransform();
	modelTransform = glm::translate(modelTransform, glm::vec3(rigidBody->GetPosition().x, rigidBody->GetPosition().y, 0.0f));
	
	modelTransform = glm::scale(modelTransform, glm::vec3(scalerToMeter, scalerToMeter, 1.0f));
	glm::mat4 viewTransform = gameState->camera->getViewTransform();
	glm::mat4 projectionTransform = gameState->camera->getProjectionTransform();
	glm::mat4 transform = projectionTransform * viewTransform * modelTransform;

	shaderProg.use();
	setUniformValue("transformMatrix", transform);
	setUniformValue("modelOriginOffset", modelOriginOffset);
	glBindVertexArray(masterVao);
	glDrawArrays(glRenderingMode, 0, modelVertexData.size());

	abortOnOpenGlError();
}

void PoGuyBox2d::getArmLocation(glm::vec3& armLocation) {
	// provide center of mass of rigid body as arm location (rope connection point)
	armLocation.x = rigidBody->GetPosition().x;
	armLocation.y = rigidBody->GetPosition().y;
	armLocation.z = 0.0f;
}

void PoGuyBox2d::setRopeAttachedToStructure(bool attachedToStructure) {
	ropeAttachedToStructure = attachedToStructure;
}

PoGuyBox2d::~PoGuyBox2d() {
	gameState->physicsManager->box2dWorld->DestroyBody(rigidBody);

	// remove camera connection constraint
	//gameState->physicsManager->dynamicsWorld->removeConstraint(cameraSpringConstraint);

	//delete cameraSpringConstraint;
	//delete physicalMass;
	//delete cameraFocalPointPhysicalMass;
}
