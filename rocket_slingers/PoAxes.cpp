#include "PoAxes.hpp"

PoAxes::PoAxes(GameState* gameState) : PhysicalObject("PO_AXES", gameState) {
	initGeometry();
	initShaders();
	initRenderData();

	shouldRender = true;
	glRenderingMode = GL_LINES;
	//inputQueue->subscribeToInputEvent(InputEvent::IEK_MOUSE_BUTTON_1, InputEvent::IEKS_PRESS, this);
	gameState->physicalObjectRenderer->addPhysicalObject(this);
}

void PoAxes::inputEventCallback(InputEvent inputEvent) {
	std::cout << "PoAxes callback.  InputEvent = " << std::endl;
	inputEvent.print();
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

	modelOriginOffsetData.clear();
	colorData.clear();
	transformData.clear();

	for (unsigned int i = 0; i < 3; i++) {
		modelOriginOffsetData.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
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