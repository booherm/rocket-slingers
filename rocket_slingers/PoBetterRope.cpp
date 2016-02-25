#include "PoBetterRope.hpp"

PoBetterRope::PoBetterRope(GameState* gameState) : PhysicalObject("PO_ROPE", gameState) {

	ropeSegmentCount = 10;

	initShaders();
	initGeometry();

	this->glRenderingMode = GL_LINES;

	gameState->eventBus->subscribeToKeyboardEvent(SDL_PRESSED, SDLK_p, this);
	gameState->eventBus->subscribeToGameEvent(Event::GameEvent::THROW_ROPE, this);
	gameState->eventBus->subscribeToGameEvent(Event::GameEvent::RELEASE_ROPE, this);

	gameState->physicalObjectRenderer->addPhysicalObject(this);
} 

void PoBetterRope::sdlInputEventCallback(const Event& eventObj) {

	if (eventObj.sdlInputEvent->key.keysym.sym == SDLK_p) {
		glm::vec3 basePosition;
		rope->getBasePosition(basePosition);

		std::cout << "base mass position: " << Utilities::vectorToString(basePosition) << std::endl;

		for (unsigned int i = 0; i < ropeSegmentCount; i++) {
			glm::vec3 massPosition;
			rope->getPivotPosition(i, massPosition);
			float angle = rope->getPivotAngle(i);
			std::cout << "   mass " << i << " position: " << Utilities::vectorToString(massPosition) << ", angle = " << angle << std::endl;

		}
	}

}

void PoBetterRope::gameEventCallback(const Event& eventObj) {
	
	if (eventObj.gameEvent == Event::GameEvent::THROW_ROPE) {
		// get reference to the player object
		player = (PoGuy*)eventObj.eventPoster;

		// remove any prior instance
		if (rope != nullptr)
			delete rope;

		// determine rope end points
		glm::vec3 connectionPointLocation(eventObj.eventWorldCoordinateX, eventObj.eventWorldCoordinateY, 0.0f);
		glm::vec3 guyArmLocation;
		player->getArmLocation(guyArmLocation);

		// determine rope length and location properties
		ropeSegmentLength = glm::distance(connectionPointLocation, guyArmLocation) / (float)ropeSegmentCount;
		glm::vec3 offset = (guyArmLocation - connectionPointLocation) / (float)ropeSegmentCount;

		// create new multibody mass originating at the connection point, terminating at player position
		glm::mat4 worldTransform;
		worldTransform = glm::translate(worldTransform, connectionPointLocation);
		rope = new PhysicalMassMultiBody();
		rope->init(gameState, 0.0f, worldTransform);
		for (unsigned int i = 0; i < ropeSegmentCount; ++i) {
			rope->pushMass(1.0f, offset, false, 0.1f, 1, 4);
		}

		// Add additional mass to represent the player.  We cannot attach the player's actual rigid body object to 
		// a multibody, so we replicate it here and disable the player rigid body.
		rope->pushMass(10.0f, glm::vec3(0.0f, 0.0f, 0.0f), true, 1.0f, 1, 4);

		// add to world
		rope->addToDynamicsWorld();
		shouldRender = true;
		shouldDoPhysicalUpdate = true;

		// play rope deployment sound effect
		gameState->audioManager->playSoundEffect(AudioManager::SoundEffectId::WHIP, 0);
	}
	else {
		// rope release

		// apply rope mass force to player object
		//btVector3 ropeForceBt = rope->multiBody->getLinkForce(ropeSegmentCount);
//		btVector3 ropeForceBt = rope->multiBody->getLink(ropeSegmentCount).m_appliedForce;
		//glm::vec3 ropeForce;
		//PhysicsManager::btVec3ToGlmVec3(ropeForceBt, ropeForce);

		btMultibodyLink playerMass = rope->multiBody->getLink(ropeSegmentCount);

		//player->addForceFromRope(currentPlayerMassForce);

		delete rope;
		rope = nullptr;

		shouldRender = false;
		shouldDoPhysicalUpdate = false;

		// play rope release sound effect
		gameState->audioManager->playSoundEffect(AudioManager::SoundEffectId::ROPE_RETRACT, 0);

	}
}

void PoBetterRope::initGeometry() {
	// model data - just a line segment that gets instanced
	modelVertices.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	modelVertices.push_back(glm::vec3(1.0f, 0.0f, 0.0f));

	// color value for max vertex count
	for (unsigned int massIndex = 0; massIndex < ropeSegmentCount; massIndex++) {
		colorData.push_back(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		colorData.push_back(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	}
}

void PoBetterRope::doPhysicalUpdate() {

	btVector3 y(0.0f, 0.0f, 0.0f);
	btVector3 z = rope->multiBody->getLink(ropeSegmentCount).m_cachedWorldTransform * y;

	glm::vec3 pos;
	PhysicsManager::btVec3ToGlmVec3(z, pos);

	glm::vec3 basePosition;
	rope->getBasePosition(basePosition);
	pos = pos - basePosition;

	glm::vec3 rocketForce;
	player->getRocketForce(rocketForce);
	float forceMagnitude = glm::length(rocketForce);

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
			if (pos.y ==  0.0f)
				theta = glm::pi<float>();
			else
				theta += glm::pi<float>();
		}
	}
	glm::quat rotationQuaternion = glm::angleAxis(theta, glm::vec3(0.0f, 0.0f, 1.0f));
	glm::vec3 radialVector = rotationQuaternion * glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 perpForce(-radialVector.y, radialVector.x, radialVector.z);
	perpForce = perpForce * forceMagnitude;

	//std::cout << "pos = " << Utilities::vectorToString(pos) << ", theta = " << theta << ", radialVector = " << Utilities::vectorToString(radialVector) << ", perpForce = " << Utilities::vectorToString(perpForce) << std::endl;

	rope->applyForceToPivot(ropeSegmentCount, perpForce);
	currentPlayerMassForce = perpForce;

}

void PoBetterRope::doRenderUpdate() {

	transformData.clear();

	glm::mat4 viewTransform = gameState->camera->getViewTransform();
	glm::mat4 projectionTransform = gameState->camera->getProjectionTransform();

	glm::vec3 lastMassPosition;
	rope->getBasePosition(lastMassPosition);

	for (unsigned int i = 0; i < ropeSegmentCount; ++i) {

		// translate to connection point location
		glm::mat4 modelTransform;
		modelTransform = glm::translate(modelTransform, lastMassPosition);
		glm::vec3 thisMassPosition;
		rope->getPivotPosition(i, thisMassPosition);

		// scale to segment length
		modelTransform = glm::scale(modelTransform, glm::vec3(ropeSegmentLength, ropeSegmentLength, 1.0f));

		// rotate to next segment pivot
		float theta = Utilities::xyAngleBetweenVectors(glm::vec3(1.0f, 0.0f, 0.0f), thisMassPosition - lastMassPosition);
		glm::quat rotationQuaternion = glm::angleAxis(theta, glm::vec3(0.0f, 0.0f, 1.0f));
		modelTransform = modelTransform * glm::toMat4(rotationQuaternion);

		// combine transform
		glm::mat4 transform = projectionTransform * viewTransform * modelTransform;
		transformData.push_back(transform);

		lastMassPosition = thisMassPosition;
	}
}

PoBetterRope::~PoBetterRope() {
	if(rope != nullptr)
		delete rope;
}
