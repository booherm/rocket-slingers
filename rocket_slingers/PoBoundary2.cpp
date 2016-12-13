#include "PoBoundary2.hpp"

PoBoundary2::PoBoundary2(const std::string& objectId, GameState* gameState, const glm::vec3& scalers, const glm::vec3& position) : PhysicalObject(objectId, gameState) {

	this->scalers = scalers;
	this->position = position;

	initShaders();
	initGeometry();
	initPhysics();
}

void PoBoundary2::initShaders() {

	shaderProg = OglShaderProgram();
	shaderProg.createVertexShaderFromFile("resources/PoBoundary2.vs");
	shaderProg.createFragmentShaderFromFile("resources/PoBoundary2.fs");
	shaderProg.build();

}

void PoBoundary2::initGeometry() {

	modelVertexData.push_back(glm::vec3(0.0f * scalers.x, 0.0f * scalers.y, 0.0f));
	modelVertexData.push_back(glm::vec3(1.0f * scalers.x, 0.0f * scalers.y, 0.0f));
	modelVertexData.push_back(glm::vec3(1.0f * scalers.x, 1.0f * scalers.y, 0.0f));
	modelVertexData.push_back(glm::vec3(0.0f * scalers.x, 0.0f * scalers.y, 0.0f));
	modelVertexData.push_back(glm::vec3(1.0f * scalers.x, 1.0f * scalers.y, 0.0f));
	modelVertexData.push_back(glm::vec3(0.0f * scalers.x, 1.0f * scalers.y, 0.0f));

	transformData.push_back(glm::mat4());

	// push color data
	for (unsigned int i = 0; i < modelVertexData.size(); i++) {
		colorData.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	}

	initModelVertexBuffer();
	initColorBuffer();
	buildVao(MODEL_VERTEX | COLOR);
	gameState->masterRenderer->addRenderableObject(this);
	shouldRender = true;
}

void PoBoundary2::initPhysics() {

	b2BodyDef rigidBodyDef;
	rigidBodyDef.type = b2_staticBody;
	//rigidBodyDef.linearDamping = 100.00f;
	rigidBodyDef.position.Set(position.x + (scalers.x / 2.0f), position.y + (scalers.y / 2.0f));
	rigidBody = gameState->physicsManager->box2dWorld->CreateBody(&rigidBodyDef);
	b2PolygonShape rigidBodyShape;
	rigidBodyShape.SetAsBox(scalers.x / 2.0f, scalers.y / 2.0f);
	b2FixtureDef rigidBodyFixtureDef;
	rigidBodyFixtureDef.shape = &rigidBodyShape;
	rigidBodyFixtureDef.friction = 0.5f;
	rigidBodyFixtureDef.userData = this;
	rigidBodyFixtureDef.filter.categoryBits = PhysicsManager::CollisionCategory::BOUNDARY;
	rigidBodyFixtureDef.filter.maskBits = gameState->physicsManager->getCollisionMask(PhysicsManager::CollisionCategory::BOUNDARY);
	rigidBody->CreateFixture(&rigidBodyFixtureDef);
}

void PoBoundary2::render() {

	glm::mat4 modelTransform;
	modelTransform = glm::translate(modelTransform, position);
	glm::mat4 viewTransform = gameState->camera->getViewTransform();
	glm::mat4 projectionTransform = gameState->camera->getProjectionTransform();
	glm::mat4 transform = projectionTransform * viewTransform * modelTransform;

	shaderProg.use();
	setUniformValue("transformMatrix", transform);
	setUniformValue("time", gameState->fFrameTimeStart);
	setUniformValue("resolution", glm::vec2(
		(gameState->resolutionWidth / gameState->worldViewportWidth) * scalers.x * 0.5f, 
		(gameState->resolutionHeight / gameState->worldViewportHeight) * scalers.y * 0.5f
		));

	glBindVertexArray(masterVao);

	glDrawArrays(glRenderingMode, 0, modelVertexData.size());

	abortOnOpenGlError();
}


PoBoundary2::~PoBoundary2() {
	gameState->physicsManager->box2dWorld->DestroyBody(rigidBody);
}
