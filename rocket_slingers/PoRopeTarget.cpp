#include "PoRopeTarget.hpp"
#include "Stage.hpp"

PoRopeTarget::PoRopeTarget(const std::string& objectId, GameState* gameState) : PhysicalObject(objectId, gameState) {
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
	float x, y;
	gameState->eventBus->getMousePosition(x, y);
	worldPosition = glm::vec3(x, y, 0.0f);

	colorData.clear();
	modelVertexData.clear();

	// test ray between player position and target position
	PoGuy* guy = (PoGuy*) gameState->activeStage->getPhysicalObject("GUY");
	glm::vec3 armLocGlm;
	guy->getArmLocation(armLocGlm);
	btVector3 armLoc;
	PhysicsManager::glmVec3ToBtVec3(armLocGlm, armLoc);
	btVector3 worldPositionBt;
	PhysicsManager::glmVec3ToBtVec3(worldPosition, worldPositionBt);

	btCollisionWorld::ClosestRayResultCallback hitResult(armLoc, worldPositionBt);
	hitResult.m_collisionFilterGroup = PhysicsManager::CollisionGroup::PLAYER;
	hitResult.m_collisionFilterMask = PhysicsManager::CollisionGroup::PLAYER | PhysicsManager::CollisionGroup::BOUNDARY | PhysicsManager::CollisionGroup::SWINGING_MASS;
	gameState->physicsManager->dynamicsWorld->rayTest(armLoc, worldPositionBt, hitResult);
	if (hitResult.hasHit()) {
		glm::vec3(hitLocationGlm);
		btVector3 hitLocation = hitResult.m_hitPointWorld;
		PhysicsManager::btVec3ToGlmVec3(hitLocation, hitLocationGlm);

		modelVertexData.push_back(armLocGlm);
		modelVertexData.push_back(hitLocationGlm);
		modelVertexData.push_back(glm::vec3(hitLocationGlm.x - 0.5f, hitLocationGlm.y, 0.0f));
		modelVertexData.push_back(glm::vec3(hitLocationGlm.x + 0.5f, hitLocationGlm.y, 0.0f));
		modelVertexData.push_back(glm::vec3(hitLocationGlm.x, hitLocationGlm.y - 0.5f, 0.0f));
		modelVertexData.push_back(glm::vec3(hitLocationGlm.x, hitLocationGlm.y + 0.5f, 0.0f));
		modelVertexData.push_back(hitLocationGlm);
		modelVertexData.push_back(worldPosition);
		modelVertexData.push_back(glm::vec3(worldPosition.x - 0.5f, worldPosition.y, 0.0f));
		modelVertexData.push_back(glm::vec3(worldPosition.x + 0.5f, worldPosition.y, 0.0f));
		modelVertexData.push_back(glm::vec3(worldPosition.x, worldPosition.y - 0.5f, 0.0f));
		modelVertexData.push_back(glm::vec3(worldPosition.x, worldPosition.y + 0.5f, 0.0f));

		for (unsigned int i = 0; i < modelVertexData.size(); i++)
			colorData.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));

		//std::cout << "hit detected at " << Utilities::vectorToString(hitLocationGlm) << std::endl;
	}
	else {

		modelVertexData.push_back(armLocGlm);
		modelVertexData.push_back(worldPosition);
		modelVertexData.push_back(glm::vec3(worldPosition.x - 0.5f, worldPosition.y, 0.0f));
		modelVertexData.push_back(glm::vec3(worldPosition.x + 0.5f, worldPosition.y, 0.0f));
		modelVertexData.push_back(glm::vec3(worldPosition.x, worldPosition.y - 0.5f, 0.0f));
		modelVertexData.push_back(glm::vec3(worldPosition.x, worldPosition.y + 0.5f, 0.0f));

		for (unsigned int i = 0; i < modelVertexData.size(); i++)
			colorData.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

	}

	//std::cout << "arm loc = " << Utilities::vectorToString(armLoc) << std::endl;
}

void PoRopeTarget::initGeometry() {

	glRenderingMode = GL_LINES;

	zDepth = 0.9f;
	initModelVertexBuffer(12);
	initColorBuffer(12);
	buildVao(MODEL_VERTEX | COLOR);
	shouldRender = true;
	gameState->masterRenderer->addRenderableObject(this);
}

void PoRopeTarget::render() {

	refreshModelVertexBuffer();
	refreshColorBuffer();
	glm::mat4 modelTransform;
//	modelTransform = glm::translate(modelTransform, worldPosition);
	glm::mat4 viewTransform = gameState->camera->getViewTransform();
	glm::mat4 projectionTransform = gameState->camera->getProjectionTransform();
	glm::mat4 transform = projectionTransform * viewTransform;// *modelTransform;

	shaderProg.use();
	setUniformValue("transformMatrix", transform);
	glBindVertexArray(masterVao);
	glDrawArrays(glRenderingMode, 0, modelVertexData.size());

	abortOnOpenGlError();
}
