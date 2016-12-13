#include "PoRope.hpp"
#include "Stage.hpp"

PoRope::PoRope(const std::string& objectId, GameState* gameState) : PhysicalObject(objectId, gameState) {

	ropeSegmentsCount = 20;
	maxRopeLength = 10.0f;
	totalRopeMass = ropeSegmentsCount * 10.0f;
	maxRopeSegmentLength = maxRopeLength / ropeSegmentsCount;
	attachedToStructure = false;
	
	initShaders();
	initGeometry();

	ropeTarget = (PoRopeTarget*) gameState->activeStage->getPhysicalObject("ROPE_TARGET");
	ropeTarget->setRopeMaxLength(maxRopeLength);

	gameState->eventBus->subscribeToGameEvent(Event::GameEvent::THROW_ROPE, this);
	gameState->eventBus->subscribeToGameEvent(Event::GameEvent::RELEASE_ROPE, this);
}

void PoRope::gameEventCallback(const Event& eventObj) {

	attachedToStructure = false;
	player = (PoGuy*) eventObj.eventPoster;

	if (eventObj.gameEvent == Event::GameEvent::THROW_ROPE) {

		// remove any prior instance
		destructRope();

		float ropeSegmentLength;
		b2Vec2 ropeTerminationPoint;

		if (ropeTarget->targetAquired) {

			// get distance between player and target
			float distance = b2Distance(ropeTarget->playerPosition, ropeTarget->targetPosition);
			if (distance <= maxRopeLength) {
				attachedToStructure = true;
				ropeTerminationPoint = ropeTarget->targetPosition;
				ropeSegmentLength = distance / (float) ropeSegmentsCount;
			}

		}

		// calculate angle between x unit vector at player arm location to connection point
		float theta = Utilities::xyAngleBetweenVectors(b2Vec2(1.0f, 0.0f), ropeTarget->mousePosition - ropeTarget->playerPosition);
		if (!attachedToStructure) {
			// no connection to environment, set rope segment length to max length and connection point to max rope length along the ray to the click point
			ropeTerminationPoint = ropeTarget->playerPosition + b2Vec2(glm::cos(theta) * maxRopeLength, glm::sin(theta) * maxRopeLength);
			ropeSegmentLength = maxRopeSegmentLength;
		}
		b2Vec2 offset = (ropeTarget->playerPosition - ropeTerminationPoint) / (float) ropeSegmentsCount;

		// create new rigid bodies connected by constraints, originating at the connection point (or max rope length point), terminating at player position
		ropeSegments.resize(ropeSegmentsCount);
		for (unsigned int i = 0; i < ropeSegmentsCount; ++i) {

			b2Vec2 segmentCenterPoint(ropeTerminationPoint.x + (offset.x * (float) i) + (0.5f * offset.x), ropeTerminationPoint.y + (offset.y * (float) i) + (0.5f * offset.y));

			b2BodyDef segmentBodyDef;
			segmentBodyDef.type = b2_dynamicBody;
			segmentBodyDef.allowSleep = false;
			b2Body* body = gameState->physicsManager->box2dWorld->CreateBody(&segmentBodyDef);
			b2PolygonShape segmentBodyShape;
			segmentBodyShape.SetAsBox(0.025f, ropeSegmentLength / 2.0f);

			b2FixtureDef segmentBodyFixtureDef;
			segmentBodyFixtureDef.shape = &segmentBodyShape;
			segmentBodyFixtureDef.density = totalRopeMass / ropeSegmentsCount;
			segmentBodyFixtureDef.friction = 0.3f;
			segmentBodyFixtureDef.filter.categoryBits = PhysicsManager::CollisionCategory::ROPE_SEGMENT;
			segmentBodyFixtureDef.filter.maskBits = gameState->physicsManager->getCollisionMask(PhysicsManager::CollisionCategory::ROPE_SEGMENT);
			segmentBodyFixtureDef.userData = this;
			body->CreateFixture(&segmentBodyFixtureDef);
			ropeSegments[i].body = body;
			body->SetTransform(segmentCenterPoint, theta - glm::half_pi<float>());
		}

		// create joint between world and attached first rope segment
		if (attachedToStructure) {

			b2RevoluteJointDef revJointDef;
			revJointDef.bodyA = ropeTarget->targetRigidBody;
			revJointDef.bodyB = ropeSegments[0].body;
			revJointDef.localAnchorA = ropeTarget->targetRigidBody->GetLocalPoint(ropeTarget->targetPosition);
			revJointDef.localAnchorB.y = ropeSegmentLength / 2.0f;
			revJointDef.collideConnected = false;
			gameState->physicsManager->box2dWorld->CreateJoint(&revJointDef);
		}

		// link created rigid bodies
		for (unsigned int i = 0; i < ropeSegmentsCount - 1; ++i) {

			// revolute joint
			b2RevoluteJointDef revJointDef;
			revJointDef.bodyA = ropeSegments[i].body;
			revJointDef.bodyB = ropeSegments[i + 1].body;
			revJointDef.localAnchorA.y = -ropeSegmentLength / 2.0f;
			revJointDef.localAnchorB.y = ropeSegmentLength / 2.0f;
			revJointDef.collideConnected = false;
			ropeSegments[i].revJoint = (b2RevoluteJoint*) gameState->physicsManager->box2dWorld->CreateJoint(&revJointDef);

		}


		// rope joints for stability
		for (unsigned int i = 1; i < ropeSegmentsCount; ++i) {
			b2RopeJointDef ropeJointDef;
			ropeJointDef.bodyA = ropeSegments[0].body;
			ropeJointDef.bodyB = ropeSegments[i].body;
			ropeJointDef.localAnchorA = b2Vec2(0.0f, 0.0f);
			ropeJointDef.localAnchorB = b2Vec2(0.0f, 0.0f);
			ropeJointDef.collideConnected = false;
			ropeJointDef.maxLength = i * ropeSegmentLength;
			gameState->physicsManager->box2dWorld->CreateJoint(&ropeJointDef);
		}

		// constrain the player mass to the end of the rope
		b2Body* playerRigidBody = player->getRigidBody();
		b2RevoluteJointDef playerRevJointDef;
		playerRevJointDef.bodyA = ropeSegments[ropeSegmentsCount - 1].body;
		playerRevJointDef.bodyB = playerRigidBody;
		playerRevJointDef.localAnchorA.y = -ropeSegmentLength / 2.0f;
		playerRevJointDef.localAnchorB.y = 0.0f;
		playerRevJointDef.collideConnected = false;
		ropeSegments[ropeSegmentsCount - 1].revJoint = (b2RevoluteJoint*) gameState->physicsManager->box2dWorld->CreateJoint(&playerRevJointDef);

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
	unsigned int vertexCount = 2 * ropeSegmentsCount;
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
	b2Vec2 b2Pos = ropeSegments[ropeSegmentsCount - 1].body->GetPosition();
	pos = glm::vec3(b2Pos.x, b2Pos.y, 0.0f);

	glm::vec3 basePosition;
	b2Vec2 b2BasePosition = ropeSegments[0].body->GetPosition();
	basePosition = glm::vec3(b2BasePosition.x, b2BasePosition.y, 0.0f);
	pos = pos - basePosition;

	// get force from player rocket
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
	perpForce = perpForce * forceMagnitude * 0.15f;

	b2Vec2 b2PerpForce(perpForce.x, perpForce.y);

//	ropeSegments[ropeSegmentsCount - 1].revJoint->GetBodyB()->ApplyLinearImpulse(b2PerpForce, b2Vec2(0.0f, 0.0f), true);
	ropeSegments[ropeSegmentsCount - 1].revJoint->GetBodyB()->ApplyForceToCenter(b2PerpForce, true);

}

void PoRope::render() {

	modelVertexData.clear();

	b2Vec2 thisJointLocation = ropeSegments[0].revJoint->GetAnchorA();
	b2Vec2 nextJointLocation = ropeSegments[0].revJoint->GetAnchorB();
	modelVertexData.push_back(glm::vec3(thisJointLocation.x, thisJointLocation.y, 0.0f));
	modelVertexData.push_back(glm::vec3(nextJointLocation.x, nextJointLocation.y, 0.0f));

	for (unsigned int i = 0; i < ropeSegmentsCount - 1; ++i) {
		thisJointLocation = ropeSegments[i].revJoint->GetAnchorB();
		nextJointLocation = ropeSegments[i + 1].revJoint->GetAnchorB();
		
		modelVertexData.push_back(glm::vec3(thisJointLocation.x, thisJointLocation.y, 0.0f));
		modelVertexData.push_back(glm::vec3(nextJointLocation.x, nextJointLocation.y, 0.0f));
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
	for (unsigned int i = 0; i < ropeSegments.size(); ++i) {
		gameState->physicsManager->box2dWorld->DestroyBody(ropeSegments[i].body);
	}
	ropeSegments.clear();
}
