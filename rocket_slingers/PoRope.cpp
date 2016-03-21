#include "PoRope.hpp"

PoRope::PoRope(const std::string& objectId, GameState* gameState) : PhysicalObject(objectId, gameState) {

	ropeSegmentsCount = 20;
	maxRopeLength = 12.0f;
	totalRopeMass = ropeSegmentsCount * 10.0f;
	maxRopeSegmentLength = maxRopeLength / ropeSegmentsCount;
	attachedToStructure = false;
	anchorRevJoint = nullptr;
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
		glm::vec3 ropeTerminationPoint;
		WorldRayCastCallback cb;
		gameState->physicsManager->box2dWorld->RayCast(&cb, b2Vec2(playerArmLocation.x, playerArmLocation.y), b2Vec2(clickLocation.x, clickLocation.y));
		if (cb.fixture != nullptr) {
			PhysicalObject* firstHit = (PhysicalObject*) cb.fixture->GetUserData();
			if (firstHit->objectId == "FLOOR" || firstHit->objectId == "LEFT_WALL" || firstHit->objectId == "RIGHT_WALL" || firstHit->objectId == "CEILING") {

				glm::vec3 hitLocation(cb.point.x, cb.point.y, 0.0f);
				std::cout << "hit detected at " << Utilities::vectorToString(hitLocation) << std::endl;

				// get distance between player and hit point
				float distance = glm::distance(playerArmLocation, hitLocation);
				if (distance <= maxRopeLength) {
					attachedToStructure = true;
					ropeTerminationPoint = hitLocation;
					ropeSegmentLength = distance / (float) ropeSegmentsCount;
				}

			}
		}

		// calculate angle between x unit vector at player arm location to connection point
		float theta = Utilities::xyAngleBetweenVectors(glm::vec3(1.0f, 0.0f, 0.0f), clickLocation - playerArmLocation);
		if (!attachedToStructure) {
			// no connection to environment, set rope segment length to max length and connection point to max rope length along the ray to the click point
			ropeTerminationPoint = playerArmLocation + glm::vec3(glm::cos(theta) * maxRopeLength, glm::sin(theta) * maxRopeLength, 0.0f);
			ropeSegmentLength = maxRopeSegmentLength;
		}
		glm::vec3 offset = (playerArmLocation - ropeTerminationPoint) / (float) ropeSegmentsCount;

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
			segmentBodyFixtureDef.filter.groupIndex = -1;
			segmentBodyFixtureDef.userData = this;
			body->CreateFixture(&segmentBodyFixtureDef);
			ropeSegments[i].body = body;
			body->SetTransform(segmentCenterPoint, theta - glm::half_pi<float>());
		}

		// create joint between world and attached first rope segment
		if (attachedToStructure || true) {
			b2RevoluteJointDef revJointDef;
			revJointDef.bodyA = gameState->physicsManager->worldStaticBody;
			revJointDef.bodyB = ropeSegments[0].body;
			revJointDef.localAnchorA = b2Vec2(ropeTerminationPoint.x, ropeTerminationPoint.y);
			revJointDef.localAnchorB.y = ropeSegmentLength / 2.0f;
			revJointDef.collideConnected = false;
			anchorRevJoint = (b2RevoluteJoint*) gameState->physicsManager->box2dWorld->CreateJoint(&revJointDef);
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
			ropeSegments[i].ropeJoint = (b2RopeJoint*) gameState->physicsManager->box2dWorld->CreateJoint(&ropeJointDef);
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

	b2Vec2 b2PerpForce(perpForce.x, perpForce.y);

	ropeSegments[ropeSegmentsCount - 1].revJoint->GetBodyB()->ApplyLinearImpulse(b2PerpForce, b2Vec2(0.0f, 0.0f), true);

}

void PoRope::render() {

	modelVertexData.clear();
	for (unsigned int i = 0; i < ropeSegmentsCount - 1; ++i) {
		b2Vec2 thisJointLocation = ropeSegments[i].revJoint->GetAnchorB();
		b2Vec2 nextJointLocation = ropeSegments[i + 1].revJoint->GetAnchorB();
		
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
	if (anchorRevJoint != nullptr) {
		gameState->physicsManager->box2dWorld->DestroyJoint(anchorRevJoint);
		anchorRevJoint = nullptr;
	}

	for (unsigned int i = 1; i < ropeSegments.size(); ++i) {
		gameState->physicsManager->box2dWorld->DestroyJoint(ropeSegments[i].ropeJoint);
	}

	for (unsigned int i = 0; i < ropeSegments.size(); ++i) {
		gameState->physicsManager->box2dWorld->DestroyJoint(ropeSegments[i].revJoint);
		gameState->physicsManager->box2dWorld->DestroyBody(ropeSegments[i].body);
	}
	ropeSegments.clear();
}
