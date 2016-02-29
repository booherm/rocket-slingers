#include "PoRope.hpp"

PoRope::PoRope(const std::string& objectId, GameState* gameState) : PhysicalObject(objectId, gameState) {

	ropeMassesCount = 20;
	initShaders();
	initGeometry();

	gameState->eventBus->subscribeToGameEvent(Event::GameEvent::THROW_ROPE, this);
	gameState->eventBus->subscribeToGameEvent(Event::GameEvent::RELEASE_ROPE, this);
}

void PoRope::gameEventCallback(const Event& eventObj) {

	if (eventObj.gameEvent == Event::GameEvent::THROW_ROPE) {
		// get reference to the player object
		player = (PoGuy*)eventObj.eventPoster;

		// remove any prior instance
		destructRope();

		// determine player position
		btVector3 guyArmLocation;
		glm::vec3 guyArmLocationGlm;
		player->getArmLocation(guyArmLocationGlm);
		PhysicsManager::glmVec3ToBtVec3(guyArmLocationGlm, guyArmLocation);

		// determine click location
		btVector3 connectionPointLocation;
		glm::vec3 connectionPointLocationGlm(eventObj.eventWorldCoordinateX, eventObj.eventWorldCoordinateY, 0.0f);
		PhysicsManager::glmVec3ToBtVec3(connectionPointLocationGlm, connectionPointLocation);




		//////////////////// collision stuff /////////////////
		unsigned int collisionObjectCount = gameState->physicsManager->dynamicsWorld->getCollisionWorld()->getNumCollisionObjects();
		std::cout << "Number of collision objects: " << collisionObjectCount << std::endl;
		btCollisionObjectArray coa = gameState->physicsManager->dynamicsWorld->getCollisionWorld()->getCollisionObjectArray();
		for (unsigned int j = 0; j < collisionObjectCount; j++) {
			btCollisionObject* co = coa.at(j);
			int collisionFlags = co->getCollisionFlags();
			int activationState = co->getActivationState();
			btScalar contactProcessingThreshold = co->getContactProcessingThreshold();
			btScalar hitFraction = co->getHitFraction();
			bool hasContactResponse = co->hasContactResponse();

			btTransform aabbTransform;
			btVector3 aabbMin;
			btVector3 aabbMax;
			co->getCollisionShape()->getAabb(aabbTransform, aabbMin, aabbMax);


			glm::vec3 aabbMinGlm;
			glm::vec3 aabbMaxGlm;
			PhysicsManager::btVec3ToGlmVec3(aabbMin, aabbMinGlm);
			PhysicsManager::btVec3ToGlmVec3(aabbMax, aabbMaxGlm);
			std::string bbMin = Utilities::vectorToString(aabbMinGlm);
			std::string bbMax = Utilities::vectorToString(aabbMaxGlm);
			
			if (co->getUserPointer() != nullptr) {
				PhysicalMass* pm = (PhysicalMass*) co->getUserPointer();
				std::cout
					<< "object " << j << " id = " << pm->id
					//<< ", collisionFlags = " << collisionFlags
					//<< ", activationState = " << activationState
					//<< ", contactProcessingThreshold = " << contactProcessingThreshold
					//<< ", hitFraction = " << hitFraction
					//<< ", hasContactResponse = " << hasContactResponse
					<< ", bbMin = " << bbMin
					<< ", bbMax = " << bbMax
					<< std::endl;
			}
			else
				std::cout
				<< "object " << j << " id = nullptr"
				//<< ", collisionFlags = " << collisionFlags
				//<< ", activationState = " << activationState
				//<< ", contactProcessingThreshold = " << contactProcessingThreshold
				//<< ", hitFraction = " << hitFraction
				//<< ", hasContactResponse = " << hasContactResponse
				<< ", bbMin = " << bbMin
				<< ", bbMax = " << bbMax
				<< std::endl;
		}

		// test for object hit along the ray to point where mouse was clicked
		btCollisionWorld::ClosestRayResultCallback hitResult(guyArmLocation, connectionPointLocation);

		//gameState->physicsManager->dynamicsWorld->getDebugDrawer()->drawLine()
		


		// detects rope through boudary
		//hitResult.m_collisionFilterGroup = 32767;
		//hitResult.m_collisionFilterMask


		// nothing
		//hitResult.m_collisionFilterGroup = 0;

		// detects rope through boudary
		//hitResult.m_collisionFilterGroup = -1;

		// detects rope through boudary
		hitResult.m_collisionFilterGroup = PhysicsManager::CollisionGroup::PLAYER;
		hitResult.m_collisionFilterMask = PhysicsManager::CollisionGroup::PLAYER | PhysicsManager::CollisionGroup::BOUNDARY | PhysicsManager::CollisionGroup::SWINGING_MASS;

		// detects rope through boudary
		//hitResult.m_collisionFilterGroup = PhysicsManager::CollisionGroup::PLAYER;

		//hitResult.m_collisionFilterGroup = PhysicsManager::CollisionGroup::PLAYER;

		//hitResult.m_collisionFilterGroup = PhysicsManager::CollisionGroup::BOUNDARY;
		//hitResult.m_collisionFilterMask = PhysicsManager::CollisionGroup::BOUNDARY;

		// nothing
		//hitResult.m_collisionFilterGroup = PhysicsManager::CollisionGroup::ROPE_MASS;
		//hitResult.m_collisionFilterMask = PhysicsManager::CollisionGroup::ROPE_MASS;

		// nothing
		//hitResult.m_collisionFilterGroup = PhysicsManager::CollisionGroup::ROPE_MASS;
		//hitResult.m_collisionFilterMask = PhysicsManager::CollisionGroup::ROPE_MASS | PhysicsManager::CollisionGroup::SWINGING_MASS;

		// nothing
		//hitResult.m_collisionFilterMask = PhysicsManager::CollisionGroup::ROPE_MASS | PhysicsManager::CollisionGroup::SWINGING_MASS | PhysicsManager::CollisionGroup::PLAYER | PhysicsManager::CollisionGroup::BOUNDARY | PhysicsManager::CollisionGroup::NO_COLLISION;

		// nothing
		//hitResult.m_collisionFilterMask = PhysicsManager::CollisionGroup::ROPE_MASS | PhysicsManager::CollisionGroup::SWINGING_MASS | PhysicsManager::CollisionGroup::PLAYER | PhysicsManager::CollisionGroup::BOUNDARY;

		gameState->physicsManager->dynamicsWorld->getCollisionWorld()->rayTest(guyArmLocation, connectionPointLocation, hitResult);
		//gameState->physicsManager->dynamicsWorld->rayTest(guyArmLocation, connectionPointLocation, hitResult);
		std::cout << "testing hit from " << Utilities::vectorToString(guyArmLocationGlm) << " to " << Utilities::vectorToString(connectionPointLocationGlm) << std::endl;
		if (hitResult.hasHit()) {
			btVector3 hitLocation = hitResult.m_hitPointWorld;
			PhysicsManager::btVec3ToGlmVec3(hitLocation, connectionPointLocationGlm);
			std::cout << "hit detected at " << Utilities::vectorToString(connectionPointLocationGlm) << std::endl;
		}




		// determine rope length and location properties
		ropeSegmentLength = glm::distance(connectionPointLocationGlm, guyArmLocationGlm) / (float) ropeMassesCount;
		glm::vec3 offset = (guyArmLocationGlm - connectionPointLocationGlm) / (float) ropeMassesCount;

		// create new rigid bodies connected by constraints, originating at the connection point, terminating at player position
		ropeMasses.resize(ropeMassesCount);
		for (unsigned int i = 0; i < ropeMassesCount; ++i) {

			glm::mat4 worldTransform;
			worldTransform = glm::translate(worldTransform, connectionPointLocationGlm + (offset * (float) i));

			PhysicalMass* ropeMass = new PhysicalMass();
			ropeMass->init("PO_ROPE", gameState, i == 0 ? 0.0f : 10.0f, worldTransform, PhysicsManager::CollisionGroup::ROPE_MASS);
			ropeMass->addCollisionShapeSphere(glm::mat4(), 0.2f);
			ropeMass->addToDynamicsWorld();

			ropeMass->rigidBody->setActivationState(DISABLE_DEACTIVATION);
			ropeMasses[i].ropeMass = ropeMass;
		}

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
		destructRope();
		shouldRender = false;
		shouldDoPhysicalUpdate = false;

		// play rope release sound effect
		gameState->audioManager->playSoundEffect(AudioManager::SoundEffectId::ROPE_RETRACT, 0);
	}
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
		gameState->physicsManager->dynamicsWorld->removeConstraint(ropeMasses[i].linkConstraint);
		delete ropeMasses[i].linkConstraint;
		delete ropeMasses[i].ropeMass;
	}
	ropeMasses.clear();
}
