#include "PoRopeTarget.hpp"
#include "Stage.hpp"

PoRopeTarget::PoRopeTarget(const std::string& objectId, GameState* gameState) : PhysicalObject(objectId, gameState) {
	ropeMaxLength = 0.0f;
	targetAquired = false;

	initShaders();
	initGeometry();
	shouldDoPhysicalUpdate = true;
}

void PoRopeTarget::initShaders() {

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

void PoRopeTarget::doPhysicalUpdate() {

	targetAquired = false;
	targetRigidBody = nullptr;
	b2Vec2 hitLocation;
	b2Vec2 maxRopeLengthPoint;

	// get mouse position in world coordinates
	float x, y;
	gameState->eventBus->getMousePosition(x, y);
	mousePosition = b2Vec2(x, y);

	// get player position
	PoGuy* player = (PoGuy*) gameState->activeStage->getPhysicalObject("GUY");
	player->getArmLocation(playerPosition);

	// cast ray between player position and mouse position
	WorldRayCaster rayCaster(gameState, PhysicsManager::CollisionCategory::ROPE_SEGMENT);
	b2Fixture* firstHit = rayCaster.getFirstFixtureHit(playerPosition, mousePosition);

	if (firstHit != nullptr) {
		// calculate distance between player and hit location
		hitLocation = rayCaster.point;
		float distance = b2Distance(playerPosition, hitLocation);

		if (distance < ropeMaxLength) {

			// player is within range of an attachable object, adjust target position to object's attachment point
			PhysicalObject* hitObject = (PhysicalObject*) firstHit->GetUserData();
			targetPosition = hitObject->ropeAttachable ? hitObject->ropeAttachmentPoint : hitLocation;
			targetAquired = true;
			targetRigidBody = rayCaster.fixture->GetBody();

		}
	}

	if (!targetAquired) {
		b2Vec2 distanceVector = mousePosition - playerPosition;
		if (distanceVector.Length() < ropeMaxLength) {
			maxRopeLengthPoint = mousePosition;
		}
		else {
			maxRopeLengthPoint = playerPosition + (distanceVector / distanceVector.Length()) * ropeMaxLength;
		}
	}

	colorData.clear();
	modelVertexData.clear();

	glm::vec3 playerLocationGlm = Utilities::b2Vec2ToGlmVec3(playerPosition);
	glm::vec3 mousePositionGlm = Utilities::b2Vec2ToGlmVec3(mousePosition);

	if (targetAquired) {

		glm::vec3 hitLocationGlm = Utilities::b2Vec2ToGlmVec3(hitLocation);
		glm::vec3 targetPositionGlm = Utilities::b2Vec2ToGlmVec3(targetPosition);

		// draw green line between player and hit location
		modelVertexData.push_back(playerLocationGlm);
		modelVertexData.push_back(hitLocationGlm);
		modelVertexData.push_back(glm::vec3(hitLocationGlm.x - 0.5f, hitLocationGlm.y, 0.0f));
		modelVertexData.push_back(glm::vec3(hitLocationGlm.x + 0.5f, hitLocationGlm.y, 0.0f));
		modelVertexData.push_back(glm::vec3(hitLocationGlm.x, hitLocationGlm.y - 0.5f, 0.0f));
		modelVertexData.push_back(glm::vec3(hitLocationGlm.x, hitLocationGlm.y + 0.5f, 0.0f));
		for (unsigned int i = 0; i < 6; i++)
			colorData.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));

		// draw yellow line between hit location and target position
		modelVertexData.push_back(hitLocationGlm);
		modelVertexData.push_back(targetPositionGlm);
		modelVertexData.push_back(glm::vec3(targetPositionGlm.x - 0.15f, targetPositionGlm.y, 0.0f));
		modelVertexData.push_back(glm::vec3(targetPositionGlm.x + 0.15f, targetPositionGlm.y, 0.0f));
		modelVertexData.push_back(glm::vec3(targetPositionGlm.x, targetPositionGlm.y - 0.15f, 0.0f));
		modelVertexData.push_back(glm::vec3(targetPositionGlm.x, targetPositionGlm.y + 0.15f, 0.0f));
		for (unsigned int i = 0; i < 6; i++)
			colorData.push_back(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));

		// draw faded green line between hit location and mouse position
		modelVertexData.push_back(hitLocationGlm);
		modelVertexData.push_back(mousePositionGlm);
		for (unsigned int i = 0; i < 2; i++)
			colorData.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.25f));

	}
	else{

		glm::vec3 maxRopeLengthPointGlm = Utilities::b2Vec2ToGlmVec3(maxRopeLengthPoint);

		// draw red line from player position to max rope length position
		modelVertexData.push_back(playerLocationGlm);
		modelVertexData.push_back(maxRopeLengthPointGlm);
		modelVertexData.push_back(glm::vec3(maxRopeLengthPointGlm.x - 0.5f, maxRopeLengthPointGlm.y, 0.0f));
		modelVertexData.push_back(glm::vec3(maxRopeLengthPointGlm.x + 0.5f, maxRopeLengthPointGlm.y, 0.0f));
		modelVertexData.push_back(glm::vec3(maxRopeLengthPointGlm.x, maxRopeLengthPointGlm.y - 0.5f, 0.0f));
		modelVertexData.push_back(glm::vec3(maxRopeLengthPointGlm.x, maxRopeLengthPointGlm.y + 0.5f, 0.0f));
		for (unsigned int i = 0; i < 6; i++)
			colorData.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

		// draw faded red line from max rope length position to mouse position
		modelVertexData.push_back(maxRopeLengthPointGlm);
		modelVertexData.push_back(mousePositionGlm);
		for (unsigned int i = 0; i < 2; i++)
			colorData.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.25f));

	}

	// draw the mouse position in white
	modelVertexData.push_back(glm::vec3(mousePositionGlm.x - 0.5f, mousePositionGlm.y, 0.0f));
	modelVertexData.push_back(glm::vec3(mousePositionGlm.x + 0.5f, mousePositionGlm.y, 0.0f));
	modelVertexData.push_back(glm::vec3(mousePositionGlm.x, mousePositionGlm.y - 0.5f, 0.0f));
	modelVertexData.push_back(glm::vec3(mousePositionGlm.x, mousePositionGlm.y + 0.5f, 0.0f));
	for (unsigned int i = 0; i < 4; i++)
		colorData.push_back(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

}

void PoRopeTarget::initGeometry() {

	glRenderingMode = GL_LINES;

	zDepth = 0.9f;
	initModelVertexBuffer(18);
	initColorBuffer(18);
	buildVao(MODEL_VERTEX | COLOR);
	shouldRender = true;
	gameState->masterRenderer->addRenderableObject(this);
}

void PoRopeTarget::render() {

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

void PoRopeTarget::setRopeMaxLength(float maxLength) {
	ropeMaxLength = maxLength;
}

