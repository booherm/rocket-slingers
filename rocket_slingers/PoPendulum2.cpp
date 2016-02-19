#include "PoPendulum2.hpp"

PoPendulum2::PoPendulum2(GameState* gameState) : PhysicalObject("PO_PENDULUM2", gameState) {
	initShaders();
	initGeometry();
	initPhysics();
	
	gameState->eventBus->subscribeToMouseButtonEvent(SDL_PRESSED, SDL_BUTTON_RIGHT, this);
}

void PoPendulum2::sdlInputEventCallback(const Event& eventObj) {
}

void PoPendulum2::initPhysics() {

	initialPosition = glm::vec3(15.0f, 10.0f, 0.0f);
	glm::mat4 worldTransform;
	worldTransform = glm::translate(worldTransform, initialPosition);

	glm::mat4 armLocalTransform;
	armLocalTransform = glm::translate(armLocalTransform, glm::vec3(0.0f, -0.4f * sizeScaler, 0.0f));

	glm::mat4 baseLocaltransform;
	baseLocaltransform = glm::translate(baseLocaltransform, glm::vec3(0.0f, -0.9f * sizeScaler, 0.0f));

	physicalMass = new PhysicalMass();
	physicalMass->init(gameState, 500.0f, worldTransform);
	physicalMass->addCollisionShapeBox(armLocalTransform, glm::vec3(0.06666666f, 0.8f, 0.0f) * sizeScaler);
	physicalMass->addCollisionShapeBox(baseLocaltransform, glm::vec3(0.6f, 0.2f, 0.0f) * sizeScaler);
	physicalMass->addToDynamicsWorld();


	/*
	// hinge constraint
	hingeConstraint = new btHingeConstraint(*(physicalMass->rigidBody), btVector3(0.0f, 0.0f, 0.0f), btVector3(0.0f, 0.0f, 1.0f));
	hingeConstraint->enableAngularMotor(true, 0.0f, 1.0f);
	hingeConstraint->setMotorTarget(0.0f, 1 / 60.0f);
	hingeConstraint->setLimit(-SIMD_PI * 0.5f, SIMD_PI * 0.5f);
	hingeConstraint->enableMotor(true);
	gameState->physicsManager->dynamicsWorld->addConstraint(hingeConstraint);
	*/

	/*
	// 6dof constraint
	btTransform constraintTransform = btTransform::getIdentity();
	swingConstraint = new btGeneric6DofSpring2Constraint(*(physicalMass->rigidBody), constraintTransform);
	swingConstraint->setLimit(0, 0.0f, 0.0f);
	swingConstraint->setLimit(1, 0.0f, 0.0f);
	swingConstraint->setLimit(2, 0.0f, 0.0f);
	swingConstraint->setLimit(3, 0.0f, 0.0f);
	swingConstraint->setLimit(4, 0.0f, 0.0f);
	swingConstraint->setLimit(5, 1.0f, -1.0f);
	swingConstraint->enableMotor(5, true);
	swingConstraint->setTargetVelocity(5, 3.f);
	swingConstraint->setMaxMotorForce(5, 10.f);
	swingConstraint->setServo(5, true);
	swingConstraint->setServoTarget(5, glm::half_pi<float>());
	gameState->physicsManager->dynamicsWorld->addConstraint(swingConstraint);
	*/

}

void PoPendulum2::initGeometry() {

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

void PoPendulum2::doPhysicalUpdate() {
	/*
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
	*/
}

void PoPendulum2::doRenderUpdate() {

	transformData.clear();

	// model transform
	glm::mat4 modelTransform;
	modelTransform = glm::translate(modelTransform, initialPosition);
	modelTransform = glm::scale(modelTransform, glm::vec3(sizeScaler, sizeScaler, 1.0f));

//	glm::quat rotationQuaternion = glm::angleAxis(pMultiBody->getJointPos(0), glm::vec3(0.0f, 0.0f, 1.0f));
//	modelTransform = modelTransform * glm::toMat4(rotationQuaternion);

	// view
	glm::mat4 viewTransform = gameState->camera->getViewTransform();

	// projection
	glm::mat4 projectionTransform = gameState->camera->getProjectionTransform();

	// combine transform
	glm::mat4 transform = projectionTransform * viewTransform * modelTransform;
	transformData.push_back(transform);
}

PoPendulum2::~PoPendulum2() {
	//gameState->physicsManager->dynamicsWorld->removeConstraint(hingeConstraint);
	//delete hingeConstraint;
	gameState->physicsManager->dynamicsWorld->removeConstraint(swingConstraint);
	delete swingConstraint;
	delete physicalMass;
}