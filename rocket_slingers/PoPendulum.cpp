#include "PoPendulum.hpp"

PoPendulum::PoPendulum(GameState* gameState) : PhysicalObject("PO_PENDULUM", gameState) {
	initGeometry();
	initShaders();

	maxAllowedChangeInTime = 0.002f;
	componentMasses.resize(2);
	mainComponentMass = &componentMasses[0];
	(&componentMasses[1])->mass = 10.0f;
	
	gameState->inputQueue->subscribeToMouseButtonEvent(SDL_PRESSED, SDL_BUTTON_RIGHT, this);
	gameState->physicalObjectRenderer->addPhysicalObject(this);
}

void PoPendulum::inputEventCallback(const SDL_Event& inputEvent){
	
	float worldX = gameState->inputQueue->eventWorldCoordinateX;
	float worldY = gameState->inputQueue->eventWorldCoordinateY;
	glm::vec3 originPosition = glm::vec3(worldX, worldY, 0.0f);
	mainComponentMass->worldPosition = originPosition;
	(&componentMasses[1])->worldPosition = originPosition + glm::vec3(1.0f, 0.0f, 0.0f);

	shouldRender = true;
	shouldDoPhysicalUpdate = true;
	clickCount++;

	if (clickCount == 1) {
		gameState->audioManager->playSoundEffect(AudioManager::SoundEffectId::RUSTY_SWING, -1);
	}

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
}

void PoPendulum::doPhysicalUpdate() {

	PhysicalMass* originMass = &componentMasses[0];
	PhysicalMass* bobMass = &componentMasses[1];
	bobMass->resetForce();

//	float opposite = glm::abs(bobMass->worldPosition.y - originMass->worldPosition.y);
	//float hypotenuse = glm::distance(originMass->worldPosition, bobMass->worldPosition);
	//float theta = glm::asin(opposite / hypotenuse) + glm::half_pi<float>();

	glm::vec3 connectionVector = originMass->worldPosition - bobMass->worldPosition;
	float stretchedLength = glm::length(connectionVector);
	glm::vec3 springForce;
	if (stretchedLength != 0) {
		springForce = -(connectionVector / stretchedLength)
			* (stretchedLength - 1.0f)
			* 10000.00f; // springStiffnessConstant;
	}

	bobMass->force += -springForce;
	bobMass->force += bobMass->mass * glm::vec3(0.0f, -9.81f, 0.0f);

	bobMass->updatePhysics(changeInTime);
}

void PoPendulum::doRenderUpdate() {

	// color
	colorData.clear();
	glm::vec4 modelColor = (clickCount % 2 == 0) ? glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) : glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	for (unsigned int i = 0; i < modelVertices.size(); i++)
		colorData.push_back(modelColor);

	// transform
	transformData.clear();

	// model
	glm::mat4 modelTransform;
	modelTransform = glm::translate(modelTransform, mainComponentMass->worldPosition);
	float theta = Utilities::xyAngleBetweenVectors(glm::vec3(1.0f, 0.0f, 0.0f), componentMasses[0].worldPosition - componentMasses[1].worldPosition) - glm::half_pi<float>();
	glm::quat rotationQuaternion = glm::angleAxis(theta, glm::vec3(0.0f, 0.0f, 1.0f));
	modelTransform = modelTransform * glm::toMat4(rotationQuaternion);

	// view
	glm::mat4 viewTransform = gameState->camera->getViewTransform();

	// projection
	glm::mat4 projectionTransform = gameState->camera->getProjectionTransform();

	// combine transform
	glm::mat4 transform = projectionTransform * viewTransform * modelTransform;
	transformData.push_back(transform);
}