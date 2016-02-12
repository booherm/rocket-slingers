#include "PoPendulum.hpp"

PoPendulum::PoPendulum(GameState* gameState) : PhysicalObject("PO_PENDULUM", gameState) {
	initGeometry();
	initShaders();

	componentMasses.resize(2);
	mainComponentMass = &componentMasses[0];
	mainComponentMass->mass = 100.00f;
	(&componentMasses[1])->mass = 100.0f;
	
	gameState->inputQueue->subscribeToInputEvent(InputEvent::IEK_MOUSE_BUTTON_3, InputEvent::IEKS_KEYDOWN, this);
	gameState->physicalObjectRenderer->addPhysicalObject(this);
}

void PoPendulum::inputEventCallback(InputEvent inputEvent){
	
	glm::vec3 originPosition = glm::vec3(inputEvent.xWorldCoordinate, inputEvent.yWorldCoordinate, 0.0f);
	mainComponentMass->worldPosition = originPosition;
	(&componentMasses[1])->worldPosition = originPosition + glm::vec3(1.0f, 0.0f, 0.0f);

	shouldRender = true;
	shouldDoPhysicalUpdate = true;
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

void PoPendulum::doPhysicalUpdate() {

	/*
	resetForces();
	for (unsigned int i = 0; i < 2; i++) {

		PhysicalMass* thisMass = &componentMasses[i];
		PhysicalMass* nextMass = nullptr;
		if (i != 1) {
			nextMass = &componentMasses[i + 1];
		}
		
		if (nextMass != nullptr) {
			// calculate and apply spring force
			glm::vec3 springVector = thisMass->worldPosition - nextMass->worldPosition;
			float distance = glm::length(springVector);


			glm::vec3 springForce;
			if (distance != 0) {
				springForce = -(springVector / distance);// *springStiffnessConstant;
			}

			// apply internal spring friction
			//springForce += -(thisMass->velocity - nextMass->velocity) * internalSpringFrictionConstant;

			// apply spring force to this rope segment
			thisMass->force += springForce;

			// apply opposite spring force to adjacent mass
			nextMass->force += -springForce;
		}

		// apply gravitational force
		thisMass->force += thisMass->mass * glm::vec3(0.0f, -9.81, 0.0f);

		// apply air friction force
		if (i != 0) {
			thisMass->velocity += ((thisMass->force / thisMass->mass) * changeInTime);
			thisMass->worldPosition += (thisMass->velocity * changeInTime);
		}
	}
	*/

}

void PoPendulum::doRenderUpdate() {

	// model origin offset
	modelOriginOffsetData.clear();
	modelOriginOffsetData.push_back(glm::vec3(-0.3f, -1.0f, 0.0f));

	// color
	colorData.clear();
	if (clickCount % 2 == 0)
		colorData.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	else
		colorData.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));

	// transform
	transformData.clear();

	// model
	glm::mat4 modelTransform;
	modelTransform = glm::translate(modelTransform, mainComponentMass->worldPosition);
	//modelTransform = glm::scale(modelTransform, glm::vec3(scalerToMeter, scalerToMeter, 1.0f));
	
	
	//float theta = glm::half_pi<float>();
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