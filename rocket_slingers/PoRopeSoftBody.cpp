#include "PoRopeSoftBody.hpp"
#include <BulletSoftBody/btSoftBodyHelpers.h>

PoRopeSoftBody::PoRopeSoftBody(const std::string& objectId, GameState* gameState) : PhysicalObject(objectId, gameState) {

	maxRopeLength = 5.0f;
	ropeSegmentsCount = 10;
	maxRopeSegmentLength = maxRopeLength / ropeSegmentsCount;
	ropeSoftBody = nullptr;

	initShaders();
	initGeometry();
	initPhysics();

	gameState->eventBus->subscribeToGameEvent(Event::GameEvent::THROW_ROPE, this);
	gameState->eventBus->subscribeToGameEvent(Event::GameEvent::RELEASE_ROPE, this);
}


void PoRopeSoftBody::gameEventCallback(const Event& eventObj) {

	player = (PoGuy*) eventObj.eventPoster;
	bool attachedToStructure = false;

	// remove any prior instance
	destructRope();

	if (eventObj.gameEvent == Event::GameEvent::THROW_ROPE) {

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
				ropeSegmentLength = distance / (float) (ropeSegmentsCount + 1);
			}
		}

		if (!attachedToStructure) {
			// no connection to environment, set rope segment length to max length and connection point
			// to max rope length along the ray to the click point
			float theta = Utilities::xyAngleBetweenVectors(glm::vec3(1.0f, 0.0f, 0.0f), clickLocation - playerArmLocation);
			ropeTerminationPoint = playerArmLocation + glm::vec3(glm::cos(theta) * maxRopeLength, glm::sin(theta) * maxRopeLength, 0.0f);
			ropeSegmentLength = maxRopeSegmentLength;
		}

		// create rope soft body
		btVector3 ropeStartPositionBt;
		btVector3 ropeEndPositionBt;
		PhysicsManager::glmVec3ToBtVec3(ropeTerminationPoint, ropeStartPositionBt);
		PhysicsManager::glmVec3ToBtVec3(playerArmLocation, ropeEndPositionBt);
		unsigned int resolution = ropeSegmentsCount - 1;
		ropeSoftBody = btSoftBodyHelpers::CreateRope(gameState->physicsManager->dynamicsWorld->getWorldInfo(), ropeStartPositionBt, ropeEndPositionBt, resolution, 1);
		//ropeSoftBody->m_cfg.piterations = 1000;  // 1500 max
		gameState->physicsManager->dynamicsWorld->addSoftBody(ropeSoftBody);

		// attach player mass to rope
		ropeSoftBody->appendAnchor(resolution + 1, player->getPhysicalMass()->rigidBody, true, 1.0f);

		// play rope deployment sound effect
		gameState->audioManager->playSoundEffect(attachedToStructure ? AudioManager::SoundEffectId::ROPE_HIT : AudioManager::SoundEffectId::ROPE_MISS, 0);
		shouldRender = true;
		shouldDoPhysicalUpdate = true;

		// debug
		attachedToStructure = true;
	}
	else {
		
		// play rope release sound effect
		gameState->audioManager->playSoundEffect(AudioManager::SoundEffectId::ROPE_RETRACT, 0);
		shouldRender = false;
		shouldDoPhysicalUpdate = false;
		ropeSoftBody = nullptr;
	}

	player->setRopeAttachedToStructure(attachedToStructure);
	shouldDoPhysicalUpdate = attachedToStructure;
}


void PoRopeSoftBody::initShaders() {

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

void PoRopeSoftBody::initGeometry() {

	glRenderingMode = GL_LINES;
	initModelVertexBuffer(10000);
	initColorBuffer(10000);
	buildVao(MODEL_VERTEX | COLOR);

	gameState->masterRenderer->addRenderableObject(this);
}

void PoRopeSoftBody::initPhysics() {
	
}

void PoRopeSoftBody::doPhysicalUpdate() {

	glm::vec3 pos;
	player->getArmLocation(pos);

	glm::vec3 basePosition;
	btVector3 basePositionBt = ropeSoftBody->m_nodes[0].m_x;
	PhysicsManager::btVec3ToGlmVec3(basePositionBt, basePosition);

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
	perpForce = perpForce * forceMagnitude;

	player->getPhysicalMass()->applyCentralImpulse(perpForce);
	
}

void PoRopeSoftBody::render() {

	modelVertexData.clear();
	colorData.clear();

	for (int i = 0; i < ropeSoftBody->m_links.size(); i++) {
		for (int j = 0; j < 2; j++) {
			modelVertexData.push_back(glm::vec3(
				ropeSoftBody->m_links[i].m_n[j]->m_x.x(),
				ropeSoftBody->m_links[i].m_n[j]->m_x.y(),
				ropeSoftBody->m_links[i].m_n[j]->m_x.z()
			));
			colorData.push_back(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}

	refreshModelVertexBuffer();
	refreshColorBuffer();

	glm::mat4 viewTransform = gameState->camera->getViewTransform();
	glm::mat4 projectionTransform = gameState->camera->getProjectionTransform();
	glm::mat4 transform = projectionTransform * viewTransform;

	shaderProg.use();
	setUniformValue("transformMatrix", transform);
	glBindVertexArray(masterVao);
	glDrawArrays(glRenderingMode, 0, modelVertexData.size());

	abortOnOpenGlError();
}

void PoRopeSoftBody::destructRope() {
	if (ropeSoftBody != nullptr) {
		gameState->physicsManager->dynamicsWorld->removeSoftBody(ropeSoftBody);
		delete ropeSoftBody;
	}
}

PoRopeSoftBody::~PoRopeSoftBody() {
	destructRope();
}
