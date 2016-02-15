#include "PoAxes.hpp"

PoAxes::PoAxes(GameState* gameState) : PhysicalObject("PO_AXES", gameState) {
	initGeometry();
	initShaders();
	initRenderData();

	shouldRender = true;
	glRenderingMode = GL_LINES;
	gameState->physicalObjectRenderer->addPhysicalObject(this);
}

void PoAxes::initGeometry() {
	// axes model data
	modelVertices.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	modelVertices.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
}

void PoAxes::pushAxisTransform(glm::mat4 model) {
	glm::mat4 view = gameState->camera->getViewTransform();
	glm::mat4 projection = gameState->camera->getProjectionTransform();
	glm::mat4 transform = projection * view * model;
	transformData.push_back(transform);
}

void PoAxes::initRenderData() {

	for (unsigned int i = 0; i < 3; i++) {
		modelOriginOffsetData.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
		colorData.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
		colorData.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
	}

	// x axis
	glm::mat4 modelX;
	modelX = glm::translate(modelX, glm::vec3(xMin, 0.0f, 0.0f));
	modelX = glm::rotate(modelX, 0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	modelX = glm::scale(modelX, glm::vec3(xMax - xMin, 1.0f, 1.0f));
	pushAxisTransform(modelX);

	// y axis
	glm::mat4 modelY;
	modelY = glm::translate(modelY, glm::vec3(0.0f, yMin, 0.0f));
	modelY = glm::rotate(modelY, glm::half_pi<float>(), glm::vec3(0.0f, 0.0f, 1.0f));
	modelY = glm::scale(modelY, glm::vec3(yMax - yMin, 1.0f, 1.0f));
	pushAxisTransform(modelY);

	// z axis
	glm::mat4 modelZ;
	modelZ = glm::translate(modelZ, glm::vec3(0.0f, 0.0f, zMin));
	modelZ = glm::rotate(modelZ, -glm::half_pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f));
	modelZ = glm::scale(modelZ, glm::vec3(zMax - zMin, 1.0f, 1.0f));
	pushAxisTransform(modelZ);
}
