#include "PoPendulum.hpp"

PoPendulum::PoPendulum(GameState* gameState) : PhysicalObject("PO_PENDULUM", gameState) {
	initShaders();
	initGeometry();
	initPhysics();

	gameState->eventBus->subscribeToMouseButtonEvent(SDL_PRESSED, SDL_BUTTON_RIGHT, this);
}

void PoPendulum::sdlInputEventCallback(const Event& eventObj) {
}

void PoPendulum::initPhysics() {

	initialPosition = glm::vec3(31.0f, 15.0f, 0.0f);
	glm::mat4 worldTransform;
	worldTransform = glm::translate(worldTransform, initialPosition);

	multiBody = new PhysicalMassMultiBody;
	multiBody->init(gameState, 0.0f, worldTransform);
	multiBody->pushMass(5.0f, glm::vec3(0.0f, -5.0f, 0.0f), true, 0.05f, 1, 4);
	multiBody->pushMass(0.00000001f, glm::vec3(0.0f, -5.0f, 0.0f), false, 0.05f, 1, 4);
	multiBody->addToDynamicsWorld();

}

void PoPendulum::initGeometry() {

	// buffer pendulum model data
	modelVertices.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	modelVertices.push_back(glm::vec3(0.6f, 0.0f, 0.0f));
	modelVertices.push_back(glm::vec3(0.6f, 0.2f, 0.0f));
	modelVertices.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	modelVertices.push_back(glm::vec3(0.6f, 0.2f, 0.0f));
	modelVertices.push_back(glm::vec3(0.0f, 0.2f, 0.0f));
	modelVertices.push_back(glm::vec3(0.26666666f, 0.2f, 0.0f));
	modelVertices.push_back(glm::vec3(0.33333333f, 0.2f, 0.0f));
	modelVertices.push_back(glm::vec3(0.33333333f, 1.0f, 0.0f));
	modelVertices.push_back(glm::vec3(0.26666666f, 0.2f, 0.0f));
	modelVertices.push_back(glm::vec3(0.33333333f, 1.0f, 0.0f));
	modelVertices.push_back(glm::vec3(0.26666666f, 1.0f, 0.0f));

	// model origin offset
	modelOriginOffsetData.push_back(glm::vec3(-0.3f, -1.0f, 0.0f));

	// color
	glm::vec4 modelColor(0.0f, 1.0f, 0.0f, 1.0f);
	for (unsigned int i = 0; i < modelVertices.size(); i++)
		colorData.push_back(modelColor);

	gameState->physicalObjectRenderer->addPhysicalObject(this);
	shouldRender = true;
}

void PoPendulum::doPhysicalUpdate() {

}

void PoPendulum::doRenderUpdate() {

	transformData.clear();

	// model transform
	glm::mat4 modelTransform;
	modelTransform = glm::translate(modelTransform, initialPosition);
	modelTransform = glm::scale(modelTransform, glm::vec3(sizeScaler, sizeScaler, 1.0f));

	glm::quat rotationQuaternion = glm::angleAxis(multiBody->getPivotAngle(0), glm::vec3(0.0f, 0.0f, 1.0f));
	modelTransform = modelTransform * glm::toMat4(rotationQuaternion);

	// view
	glm::mat4 viewTransform = gameState->camera->getViewTransform();

	// projection
	glm::mat4 projectionTransform = gameState->camera->getProjectionTransform();

	// combine transform
	glm::mat4 transform = projectionTransform * viewTransform * modelTransform;
	transformData.push_back(transform);


	

}

PoPendulum::~PoPendulum() {
	delete multiBody;
}