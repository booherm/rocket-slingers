#include "PoRigidBodyRope.hpp"

PoRigidBodyRope::PoRigidBodyRope(GameState* gameState) : PhysicalObject("PO_RIGID_BODY_ROPE", gameState) {

	ropeMassesCount = 20;

	initShaders();
	initGeometry();

	this->glRenderingMode = GL_LINES;

	gameState->eventBus->subscribeToGameEvent(Event::GameEvent::THROW_ROPE, this);
	gameState->eventBus->subscribeToGameEvent(Event::GameEvent::RELEASE_ROPE, this);
	gameState->physicalObjectRenderer->addPhysicalObject(this);
}

void PoRigidBodyRope::gameEventCallback(const Event& eventObj) {

	if (eventObj.gameEvent == Event::GameEvent::THROW_ROPE) {
		// get reference to the player object
		player = (PoGuy*)eventObj.eventPoster;

		// remove any prior instance
		destructRope();

		// determine rope end points
		glm::vec3 connectionPointLocation(eventObj.eventWorldCoordinateX, eventObj.eventWorldCoordinateY, 0.0f);
		glm::vec3 guyArmLocation;
		player->getArmLocation(guyArmLocation);

		// determine rope length and location properties
		ropeSegmentLength = glm::distance(connectionPointLocation, guyArmLocation) / (float) ropeMassesCount;
		glm::vec3 offset = (guyArmLocation - connectionPointLocation) / (float) ropeMassesCount;

		// create new rigid bodies connected by constraints, originating at the connection point, terminating at player position
		ropeMasses.resize(ropeMassesCount);
		for (unsigned int i = 0; i < ropeMassesCount; ++i) {

			glm::mat4 worldTransform;
			worldTransform = glm::translate(worldTransform, connectionPointLocation + (offset * (float) i));

			PhysicalMass* ropeMass = new PhysicalMass();
			ropeMass->init(gameState, i == 0 ? 0.0f : 10.0f, worldTransform, PhysicsManager::CollisionGroup::ROPE_MASS);
			ropeMass->addCollisionShapeSphere(glm::mat4(), 0.2f);
			ropeMass->addToDynamicsWorld();

			ropeMasses[i].ropeMass = ropeMass;
		}

		ropeMasses[0].ropeMass->rigidBody->setActivationState(DISABLE_DEACTIVATION);
		ropeMasses[1].ropeMass->rigidBody->setActivationState(DISABLE_DEACTIVATION);

		for (unsigned int i = 0; i < ropeMassesCount - 1; ++i) {

			// link created masses
			btTransform constraintTransform = btTransform::getIdentity();

			btHingeConstraint* linkConstraint = new btHingeConstraint(
				*(ropeMasses[i].ropeMass->rigidBody),
				*(ropeMasses[i + 1].ropeMass->rigidBody),
				btVector3(0.0f, 0.0f, 0.0f),
				btVector3(ropeSegmentLength, 0.0f, 0.0f),
				btVector3(0.0f, 0.0f, 1.0f),
				btVector3(0.0f, 0.0f, 0.0f),
				true
			);

			gameState->physicsManager->dynamicsWorld->addConstraint(linkConstraint);
			ropeMasses[i].linkConstraint = linkConstraint;

		}

		// constrain the player mass to the end of the rope
		btHingeConstraint* playerRopeConstraint = new btHingeConstraint(
			*(ropeMasses[ropeMassesCount - 1].ropeMass->rigidBody),
			*(player->getRigidBody()),
			btVector3(0.0f, 0.0f, 0.0f),
			btVector3(ropeSegmentLength, 0.0f, 0.0f),
			btVector3(0.0f, 0.0f, 1.0f),
			btVector3(0.0f, 0.0f, 0.0f),
			true
		);
		gameState->physicsManager->dynamicsWorld->addConstraint(playerRopeConstraint);
		ropeMasses[ropeMassesCount - 1].linkConstraint = playerRopeConstraint;

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
/*
		btMultibodyLink playerMass = rope->multiBody->getLink(ropeSegmentCount);

		player->addForceFromRope(currentPlayerMassForce);

		delete rope;
		rope = nullptr;
		*/

		destructRope();
		shouldRender = false;
		shouldDoPhysicalUpdate = false;

		// play rope release sound effect
		gameState->audioManager->playSoundEffect(AudioManager::SoundEffectId::ROPE_RETRACT, 0);
	}
}

void PoRigidBodyRope::initGeometry() {
	// model data - just a line segment that gets instanced
	modelVertices.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	modelVertices.push_back(glm::vec3(1.0f, 0.0f, 0.0f));

	// color value for max vertex count
	for (unsigned int massIndex = 0; massIndex < ropeMassesCount - 1; massIndex++) {
		colorData.push_back(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		colorData.push_back(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	}
}

void PoRigidBodyRope::doPhysicalUpdate() {

	glm::vec3 pos;
	ropeMasses[ropeMassesCount - 1].ropeMass->getCenterOfMassPosition(pos);

	glm::vec3 basePosition;
	ropeMasses[0].ropeMass->getCenterOfMassPosition(basePosition);
	pos = pos - basePosition;

	glm::vec3 rocketForce;
	player->getRocketForce(rocketForce);
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
	perpForce = perpForce * forceMagnitude;

	//std::cout << "pos = " << Utilities::vectorToString(pos) << ", theta = " << theta << ", radialVector = " << Utilities::vectorToString(radialVector) << ", perpForce = " << Utilities::vectorToString(perpForce) << std::endl;

	btVector3 perpForceBt;
	PhysicsManager::glmVec3ToBtVec3(perpForce, perpForceBt);
	ropeMasses[ropeMassesCount - 1].ropeMass->rigidBody->applyCentralForce(perpForceBt);
}

void PoRigidBodyRope::doRenderUpdate() {

	transformData.clear();

	glm::mat4 viewTransform = gameState->camera->getViewTransform();
	glm::mat4 projectionTransform = gameState->camera->getProjectionTransform();

	for (unsigned int i = 0; i < ropeMassesCount - 1; ++i) {

		// translate to connection point location
		glm::vec3 thisMassPosition;
		ropeMasses[i].ropeMass->getCenterOfMassPosition(thisMassPosition);
		glm::mat4 modelTransform;
		modelTransform = glm::translate(modelTransform, thisMassPosition);

		// locate next mass down the rope
		glm::vec3 nextMassPosition;
		ropeMasses[i + 1].ropeMass->getCenterOfMassPosition(nextMassPosition);

		// scale to distance
		float segmentLength = glm::distance(thisMassPosition, nextMassPosition);
		modelTransform = glm::scale(modelTransform, glm::vec3(segmentLength, segmentLength, 1.0f));

		// rotate to next segment pivot
		float theta = Utilities::xyAngleBetweenVectors(glm::vec3(1.0f, 0.0f, 0.0f), nextMassPosition - thisMassPosition);
		glm::quat rotationQuaternion = glm::angleAxis(theta, glm::vec3(0.0f, 0.0f, 1.0f));
		modelTransform = modelTransform * glm::toMat4(rotationQuaternion);

		// combine transform
		glm::mat4 transform = projectionTransform * viewTransform * modelTransform;
		transformData.push_back(transform);

	}
}

PoRigidBodyRope::~PoRigidBodyRope() {
	destructRope();
}

void PoRigidBodyRope::destructRope() {
	for (unsigned int i = 0; i < ropeMasses.size(); ++i) {
		gameState->physicsManager->dynamicsWorld->removeConstraint(ropeMasses[i].linkConstraint);
		delete ropeMasses[i].linkConstraint;
		delete ropeMasses[i].ropeMass;
	}
	ropeMasses.clear();
}
