#include "PoReticule.hpp"

PoReticule::PoReticule(GameState* gameState) : PhysicalObject("PO_RETICULE", gameState) {
	initShaders();
	initGeometry();
	initRenderData();

	shouldRender = true;
	glRenderingMode = GL_LINES;

	gameState->eventBus->subscribeToGameEvent(Event::GameEvent::MOVE_RETICULE, this);
	gameState->physicalObjectRenderer->addPhysicalObject(this);
}

void PoReticule::gameEventCallback(const Event& eventObj) {
	worldPosition = glm::vec3(eventObj.eventWorldCoordinateX, eventObj.eventWorldCoordinateY, 0.0f);
}

void PoReticule::initGeometry() {
	modelVertices.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	modelVertices.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
}

void PoReticule::initRenderData() {

	for (unsigned int i = 0; i < 2; i++) {
		colorData.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
		colorData.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
	}

	setupTransforms();
}

void PoReticule::setupTransforms() {
	transformData.clear();
	glm::mat4 view = gameState->camera->getViewTransform();
	glm::mat4 projection = gameState->camera->getProjectionTransform();

	// horizontal line
	glm::mat4 modelHorz;
	modelHorz = glm::translate(modelHorz, glm::vec3(-0.5f, 0.0f, 0.0f) + worldPosition);
	modelHorz = glm::rotate(modelHorz, 0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 transformHorz = projection * view * modelHorz;
	transformData.push_back(transformHorz);

	// vertical line
	glm::mat4 modelVert;
	modelVert = glm::translate(modelVert, glm::vec3(0.0f, -0.5f, 0.0f) + worldPosition);
	modelVert = glm::rotate(modelVert, glm::half_pi<float>(), glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 transformVert = projection * view * modelVert;
	transformData.push_back(transformVert);
}

void PoReticule::doRenderUpdate() {
	setupTransforms();
}
