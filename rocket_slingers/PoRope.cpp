#include "PoRope.hpp"

PoRope::PoRope(const std::string& objectId, GameState* gameState) : PhysicalObject(objectId, gameState) {

	ropeMassesCount = 20;
	maxRopeLength = 8.0f;
	maxRopeSegmentLength = maxRopeLength / (ropeMassesCount - 1);
	attachedToStructure = false;
	initShaders();
	initGeometry();

	gameState->eventBus->subscribeToGameEvent(Event::GameEvent::THROW_ROPE, this);
	gameState->eventBus->subscribeToGameEvent(Event::GameEvent::RELEASE_ROPE, this);
}

void PoRope::gameEventCallback(const Event& eventObj) {

	attachedToStructure = false;
	player = (PoGuy*) eventObj.eventPoster;

	if (eventObj.gameEvent == Event::GameEvent::THROW_ROPE) {

		// remove any prior instance
		destructRope();

		// determine player position
		glm::vec3 playerArmLocation;
		player->getArmLocation(playerArmLocation);

		// determine click location
		glm::vec3 clickLocation(eventObj.eventWorldCoordinateX, eventObj.eventWorldCoordinateY, 0.0f);

		// test for object hit along the ray to point where mouse was clicked
		float ropeSegmentLength;
		glm::vec3 hitLocation;
		glm::vec3 ropeTerminationPoint;
		if (gameState->physicsManager->testRayHit(playerArmLocation, clickLocation, PhysicsManager::CollisionGroup::PLAYER, hitLocation)) {

			std::cout << "hit detected at " << Utilities::vectorToString(hitLocation) << std::endl;

			// get distance between player and hit point
			float distance = glm::distance(playerArmLocation, hitLocation);
			if (distance <= maxRopeLength) {
				attachedToStructure = true;
				ropeTerminationPoint = hitLocation;
				ropeSegmentLength = distance / (float) ropeMassesCount;
			}
		}

		if (!attachedToStructure) {
			// no connection to environment, set rope segment length to max length and connection point
			// to max rope length along the ray to the click point
			float theta = Utilities::xyAngleBetweenVectors(glm::vec3(1.0f, 0.0f, 0.0f), clickLocation - playerArmLocation);
			ropeTerminationPoint = playerArmLocation + glm::vec3(glm::cos(theta) * maxRopeLength, glm::sin(theta) * maxRopeLength, 0.0f);
			ropeSegmentLength = maxRopeSegmentLength;
		}

		glm::vec3 offset = (playerArmLocation - ropeTerminationPoint) / (float) (ropeMassesCount - 1);

		// create new rigid bodies connected by constraints, originating at the connection point (or max rope length point), terminating at player position
		ropeMasses.resize(ropeMassesCount);
		for (unsigned int i = 0; i < ropeMassesCount; ++i) {

			glm::mat4 worldTransform;
			worldTransform = glm::translate(worldTransform, ropeTerminationPoint + (offset * (float) i));

			PhysicalMass* ropeMass = new PhysicalMass();
			//ropeMass->init("PO_ROPE", gameState, i == 0 && attachedToStructure ? 0.0f : (100.0f / (i + 1)) , worldTransform, PhysicsManager::CollisionGroup::ROPE_MASS);
			//ropeMass->init("PO_ROPE", gameState, i == 0 && attachedToStructure ? 0.0f : 1.0f, worldTransform, PhysicsManager::CollisionGroup::ROPE_MASS);
			ropeMass->init("PO_ROPE", gameState, i == 0 ? 0.0f : 1.0f, worldTransform, PhysicsManager::CollisionGroup::ROPE_MASS);
			//ropeMass->init("PO_ROPE", gameState, i == 0 && attachedToStructure ? 0.0f : 10.0f, worldTransform, PhysicsManager::CollisionGroup::ROPE_MASS);
//			ropeMass->init("PO_ROPE", gameState, i == 0 ? 0.0f : 10.0f, worldTransform, PhysicsManager::CollisionGroup::ROPE_MASS);
			//ropeMass->init("PO_ROPE", gameState, 0.0f, worldTransform, PhysicsManager::CollisionGroup::ROPE_MASS);
			ropeMass->addCollisionShapeSphere(glm::mat4(), 0.2f);
			ropeMass->addToDynamicsWorld();

			ropeMass->rigidBody->setActivationState(DISABLE_DEACTIVATION);
			ropeMasses[i].ropeMass = ropeMass;
		}

		btVector3 offsetBt;
		PhysicsManager::glmVec3ToBtVec3(offset, offsetBt);
		// link created masses
		for (unsigned int i = 0; i < ropeMassesCount - 1; ++i) {
			btTransform constraintTransform = btTransform::getIdentity();
			btHingeConstraint* linkConstraint = new btHingeConstraint(
				*(ropeMasses[i].ropeMass->rigidBody),
				*(ropeMasses[i + 1].ropeMass->rigidBody),
				btVector3(0.0f, 0.0f, 0.0f),
				//btVector3(ropeSegmentLength, 0.0f, 0.0f),
				-offsetBt,
				//btVector3(0.0f, 0.0f, 0.0f),
				btVector3(0.0f, 0.0f, 1.0f),
				btVector3(0.0f, 0.0f, 0.0f),
				false
			);
			//linkConstraint->setLimit(0.0f, 0.05f, 0.0f, 0.3f, 0.0f);

			gameState->physicsManager->dynamicsWorld->addConstraint(linkConstraint);
			ropeMasses[i].linkConstraint = linkConstraint;
		}

		
		// constrain the player mass to the end of the rope
		PhysicalMass* playerPhysicalMass = player->getPhysicalMass();
		glm::vec3 playerCom;
		playerPhysicalMass->getCenterOfMassPosition(playerCom);
		std::cout << "playerCom = " << Utilities::vectorToString(playerCom) << std::endl;
		btHingeConstraint* playerRopeConstraint = new btHingeConstraint(
			*(ropeMasses[ropeMassesCount - 1].ropeMass->rigidBody),
			*(playerPhysicalMass->rigidBody),
			btVector3(0.0f, 0.0f, 0.0f),
			btVector3(0.0f, 0.0f, 0.0f),
			btVector3(0.0f, 0.0f, 1.0f),
			btVector3(0.0f, 0.0f, 0.0f),
			false
		);
		gameState->physicsManager->dynamicsWorld->addConstraint(playerRopeConstraint);
		ropeMasses[ropeMassesCount - 1].linkConstraint = playerRopeConstraint;
		
		attachedToStructure = true;
		shouldRender = true;
		shouldDoPhysicalUpdate = attachedToStructure;

		// play rope deployment sound effect
		gameState->audioManager->playSoundEffect(attachedToStructure ? AudioManager::SoundEffectId::ROPE_HIT : AudioManager::SoundEffectId::ROPE_MISS, 0);
	}
	else {
		// rope release
		destructRope();
		shouldRender = false;
		shouldDoPhysicalUpdate = false;

		// play rope release sound effect
		gameState->audioManager->playSoundEffect(AudioManager::SoundEffectId::ROPE_RETRACT, 0);
	}

	player->setRopeAttachedToStructure(attachedToStructure);
}

void PoRope::initShaders() {

	// vertex shader
	std::string vertexShaderSource =
		"#version 330 core\n"
		"\n"
		"layout (location = 0) in vec3 modelVertex;\n"
		"layout (location = 1) in vec4 colorValue;\n"
		"\n"
		"out vec4 fragColor;\n"
		"\n"
		"uniform mat4 transformMatrix;\n"
		"\n"
		"void main()\n"
		"{\n"
		"    gl_Position = transformMatrix * vec4(modelVertex, 1.0f);\n"
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

void PoRope::initGeometry() {

	glRenderingMode = GL_LINES;
	unsigned int vertexCount = 2 * (ropeMassesCount - 1);
	initModelVertexBuffer(vertexCount);

	for (unsigned int i = 0; i < vertexCount; i++) {
		colorData.push_back(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	}
	initColorBuffer();
	
	buildVao(MODEL_VERTEX | COLOR);
	gameState->masterRenderer->addRenderableObject(this);
}

void PoRope::doPhysicalUpdate() {

	glm::vec3 pos;
	ropeMasses[ropeMassesCount - 1].ropeMass->getCenterOfMassPosition(pos);

	glm::vec3 basePosition;
	ropeMasses[0].ropeMass->getCenterOfMassPosition(basePosition);
	pos = pos - basePosition;

	// get impulse from player rocket
	glm::vec3 rocketForce;
	player->getRocketImpulse(rocketForce);
	float forceMagnitude = glm::length(rocketForce);
	if (rocketForce.x > 0.0f)
		forceMagnitude = -forceMagnitude;

	// rotate x normal vector to tangent position
	float theta;
	if (pos.x == 0.0f) {
		if (pos.y > 0.0f)
			theta = glm::half_pi<float>();
		else
			theta = glm::three_over_two_pi<float>();
	}
	else {
		theta = glm::atan(pos.y / pos.x);

		if (pos.x > 0.0f) {
			if (pos.y < 0.0f)
				theta += glm::two_pi<float>();
		}
		else {
			if (pos.y == 0.0f)
				theta = glm::pi<float>();
			else
				theta += glm::pi<float>();
		}
	}
	glm::quat rotationQuaternion = glm::angleAxis(theta, glm::vec3(0.0f, 0.0f, 1.0f));
	glm::vec3 radialVector = rotationQuaternion * glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 perpForce(-radialVector.y, radialVector.x, radialVector.z);
	perpForce = perpForce * forceMagnitude * 0.1f;

	//std::cout << "pos = " << Utilities::vectorToString(pos) << ", theta = " << theta << ", radialVector = " << Utilities::vectorToString(radialVector) << ", perpForce = " << Utilities::vectorToString(perpForce) << std::endl;

	btVector3 perpForceBt;
	PhysicsManager::glmVec3ToBtVec3(perpForce, perpForceBt);
	//ropeMasses[ropeMassesCount - 1].ropeMass->rigidBody->applyCentralForce(perpForceBt);
	ropeMasses[ropeMassesCount - 1].ropeMass->rigidBody->applyCentralImpulse(perpForceBt);
}

void PoRope::render() {

	modelVertexData.clear();
	for (unsigned int i = 0; i < ropeMassesCount - 1; ++i) {
		glm::vec3 thisMassPosition;
		ropeMasses[i].ropeMass->getCenterOfMassPosition(thisMassPosition);
		modelVertexData.push_back(thisMassPosition);

		glm::vec3 nextMassPosition;
		ropeMasses[i + 1].ropeMass->getCenterOfMassPosition(nextMassPosition);
		modelVertexData.push_back(nextMassPosition);
	}

	refreshModelVertexBuffer();
	
	glm::mat4 viewTransform = gameState->camera->getViewTransform();
	glm::mat4 projectionTransform = gameState->camera->getProjectionTransform();
	glm::mat4 transform = projectionTransform * viewTransform;

	shaderProg.use();
	setUniformValue("transformMatrix", transform);
	glBindVertexArray(masterVao);
	glDrawArrays(glRenderingMode, 0, modelVertexData.size());

	abortOnOpenGlError();
}

PoRope::~PoRope() {
	destructRope();
}

void PoRope::destructRope() {
	for (unsigned int i = 0; i < ropeMasses.size(); ++i) {
//		if (i != ropeMasses.size() - 1) {
			gameState->physicsManager->dynamicsWorld->removeConstraint(ropeMasses[i].linkConstraint);
			delete ropeMasses[i].linkConstraint;
	//	}
		delete ropeMasses[i].ropeMass;
	}
	ropeMasses.clear();
}
