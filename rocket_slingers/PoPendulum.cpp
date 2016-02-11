#include "PoPendulum.hpp"

PoPendulum::PoPendulum(GameState* gameState) : PhysicalObject("PO_PENDULUM", gameState) {
	initGeometry();
	initShaders();
	shouldRender = false;
	gameState->inputQueue->subscribeToInputEvent(InputEvent::IEK_MOUSE_BUTTON_2, InputEvent::IEKS_PRESS, this);
	gameState->physicalObjectRenderer->addPhysicalObject(this);
}

void PoPendulum::inputEventCallback(InputEvent inputEvent){
	std::cout << "this is the call back! inputEvent = " << std::endl;
	inputEvent.print();

	float worldX = ((float) inputEvent.xCoordinate * gameState->aspectRatio) / gameState->resolutionWidth;
	float worldY = 1.0f - ((float) inputEvent.yCoordinate / gameState->resolutionHeight);
	std::cout << "world space coords (" << worldX << ", " << worldY << ")" << std::endl;

	worldPosition.x = worldX;
	worldPosition.y = worldY;
	shouldRender = true;
	clickCount++;
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
}

//		"    gl_Position = transformMatrix * vec4(position.x + -0.3f, position.y + -1.0f, 0.0f, 1.0f);\n" // applying origin shift here

void PoPendulum::updatePhysicalState() {

	// model origin offset
	modelOriginOffsetData.clear();
	modelOriginOffsetData.push_back(glm::vec3(-0.3f, -1.0f, 0.0f));

	// color
	colorData.clear();
	if(clickCount % 2 == 0)
		colorData.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	else
		colorData.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));

	// transform
	transformData.clear();
	// I want the object in world space to be 2m tall.  Assuming the model to be 1m tall, scaling factor =
	GLfloat scalingFactor = 2.0f * gameState->meterToWorldUnitScalar;

	// model
	glm::mat4 modelTransform;
	modelTransform = glm::translate(modelTransform, worldPosition);
	modelTransform = glm::scale(modelTransform, glm::vec3(scalingFactor, scalingFactor, 1.0f));
//	float theta = glm::cos( glm::sqrt(9.8f) * elapsedTime) * (1.0f / elapsedTime);
	float theta = glm::half_pi<float>();
	//glm::quat rotationQuaternion = glm::angleAxis(glm::sin((float) gameState->frameTimeStart), glm::vec3(0.0f, 0.0f, 1.0f));
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
