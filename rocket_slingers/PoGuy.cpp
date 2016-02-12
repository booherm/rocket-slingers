#include "PoGuy.hpp"

PoGuy::PoGuy(GameState* gameState) : PhysicalObject("PO_GUY", gameState) {
	initGeometry();
	initShaders();

	shouldRender = true;
	shouldDoPhysicalUpdate = true;
	componentMasses.resize(1);
	mainComponentMass = &componentMasses[0];

	mainComponentMass->mass = 62.00f;
	mainComponentMass->worldPosition = glm::vec3(0.25f, 0.75f, 0.0f);

	gameState->inputQueue->subscribeToKeyboardEvent(SDL_PRESSED, SDLK_SPACE, this);
	gameState->inputQueue->subscribeToKeyboardEvent(SDL_RELEASED, SDLK_SPACE, this);
	gameState->inputQueue->subscribeToKeyboardEvent(SDL_PRESSED, SDLK_g, this);
	gameState->physicalObjectRenderer->addPhysicalObject(this);
}

void PoGuy::inputEventCallback(const SDL_Event& inputEvent) {

	if (inputEvent.key.keysym.sym == SDLK_SPACE) {

		rocketOn = inputEvent.key.state == SDL_PRESSED;
		std::cout << "rocket " << (rocketOn ? "on" : "off") << std::endl;
	}
	else
		((PhysicalMass*) &componentMasses[0])->worldPosition = glm::vec3(0.25f, 0.75f, 0.0f);
}

void PoGuy::initGeometry() {

	// buffer axes model data
	float guyModelVerticesSet[57][2] = {
		{ 0.0f, 0.0f },
		{ 0.0f, 5.0f },
		{ 2.0f, 4.0f },
		{ 2.5f, 5.0f },
		{ 3.0f, 4.0f },
		{ 6.0f, 4.0f },
		{ 7.0f, 8.0f },
		{ 8.0f, 11.0f },
		{ 8.0f, 15.0f },
		{ 7.0f, 16.0f },
		{ 7.0f, 18.0f },
		{ 7.5f, 20.0f },
		{ 11.0f, 28.0f },
		{ 11.5f, 29.0f },
		{ 11.0f, 31.0f },
		{ 10.0f, 33.0f },
		{ 9.0f, 35.5f },
		{ 10.0f, 36.5f },
		{ 15.0f, 40.0f },
		{ 15.5f, 41.5f },
		{ 15.5f, 43.0f },
		{ 16.0f, 45.0f },
		{ 17.0f, 46.0f },
		{ 18.5f, 46.5f },
		{ 20.0f, 46.0f },
		{ 21.0f, 45.0f },
		{ 21.5f, 43.0f },
		{ 21.0f, 40.0f },
		{ 24.0f, 37.0f },
		{ 26.0f, 33.0f },
		{ 25.5f, 31.0f },
		{ 23.5f, 28.5f },
		{ 26.0f, 20.0f },
		{ 28.0f, 8.0f },
		{ 29.5f, 6.0f },
		{ 30.0f, 4.0f },
		{ 33.0f, 4.0f },
		{ 35.0f, 2.5f },
		{ 33.0f, 1.0f },
		{ 3.0f, 1.0f },
		{ 2.5f, 0.0f },
		{ 2.0f, 1.0f },
		{ 10.5f, 4.0f },
		{ 9.5f, 5.0f },
		{ 9.5f, 8.0f },
		{ 11.5f, 12.0f },
		{ 12.0f, 16.0f },
		{ 14.0f, 20.0f },
		{ 16.0f, 22.0f },
		{ 17.0f, 22.0f },
		{ 18.5f, 19.0f },
		{ 19.0f, 16.0f },
		{ 20.0f, 15.0f },
		{ 21.5f, 14.5f },
		{ 22.5f, 13.5f },
		{ 24.0f, 9.0f },
		{ 24.5f, 4.0f }
	};

	unsigned int guyModelTriangleVertexIds[53][3] = {
		{ 0,41,1 },
		{ 41,2,1 },
		{ 41,4,2 },
		{ 2,4,3 },
		{ 40,39,41 },
		{ 41,39,4 },
		{ 39,36,4 },
		{ 39,38,36 },
		{ 38,37,36 },
		{ 5,42,43 },
		{ 5,43,44 },
		{ 5,44,6 },
		{ 6,44,7 },
		{ 44,45,7 },
		{ 7,45,8 },
		{ 45,46,8 },
		{ 8,46,9 },
		{ 9,46,10 },
		{ 46,47,10 },
		{ 10,47,11 },
		{ 11,47,48 },
		{ 11,48,12 },
		{ 48,49,12 },
		{ 49,32,12 },
		{ 32,31,12 },
		{ 12,31,13 },
		{ 13,31,30 },
		{ 13,30,14 },
		{ 14,30,15 },
		{ 30,29,15 },
		{ 15,29,16 },
		{ 16,29,17 },
		{ 29,28,17 },
		{ 17,28,18 },
		{ 28,27,18 },
		{ 18,27,19 },
		{ 27,26,19 },
		{ 19,26,20 },
		{ 20,26,25 },
		{ 20,25,21 },
		{ 21,25,24 },
		{ 21,24,22 },
		{ 22,24,23 },
		{ 50,32,49 },
		{ 51,32,50 },
		{ 52,32,51 },
		{ 53,32,52 },
		{ 54,32,53 },
		{ 33,32,54 },
		{ 55,33,54 },
		{ 56,33,55 },
		{ 56,34,33 },
		{ 56,35,34 }
	};

	unsigned int guyModelVertexIndex = 0;
	for (unsigned int t = 0; t < 53; t++) {
		for (unsigned int v = 0; v < 3; v++) {
			modelVertices.push_back(glm::vec3(guyModelVerticesSet[guyModelTriangleVertexIds[t][v]][0], guyModelVerticesSet[guyModelTriangleVertexIds[t][v]][1], 0.0f));
		}
	}
}

void PoGuy::doPhysicalUpdate() {

	mainComponentMass->resetForce();

	// rocket force
	if (rocketOn)
		mainComponentMass->force += glm::vec3(100.0f, 0.0f, 0.0f);

	// air friction
	mainComponentMass->force += (mainComponentMass->velocity * -airFrictionConstant);

	mainComponentMass->updatePhysics(changeInTime);
}

void PoGuy::doRenderUpdate() {

	modelOriginOffsetData.clear();
	colorData.clear();
	transformData.clear();

	modelOriginOffsetData.push_back(glm::vec3(0.0f, 0.0f, 0.0f));

	// color
	colorData.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

	// model transform: translate, scale, rotate
	glm::mat4 modelTransform;
	modelTransform = glm::translate(modelTransform, mainComponentMass->worldPosition);
	modelTransform = glm::scale(modelTransform, glm::vec3(scalerToMeter, scalerToMeter, 1.0f));

	// view
	glm::mat4 viewTransform = gameState->camera->getViewTransform();

	// projection
	glm::mat4 projectionTransform = gameState->camera->getProjectionTransform();

	// combine transform
	glm::mat4 transform = projectionTransform * viewTransform * modelTransform;
	transformData.push_back(transform);

}