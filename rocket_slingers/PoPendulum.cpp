#include "PoPendulum.hpp"

PoPendulum::PoPendulum(GameState* gameState) : PhysicalObject("PO_PENDULUM", gameState) {

	initialPosition = glm::vec3(15.0f, 15.0f, 0.0f);

	initShaders();
	initGeometry();
	initPhysics();

	shouldDoPhysicalUpdate = false;
}

void PoPendulum::initGeometry() {

	// pendulum model points
	std::vector<glm::vec3> points;
	points.push_back(glm::vec3(-0.5f, -0.15f, 0.0f)); // A = 0
	points.push_back(glm::vec3(0.5f, -0.15f, 0.0f));  // B = 1
	points.push_back(glm::vec3(0.5f, 0.15f, 0.0f));   // C = 2
	points.push_back(glm::vec3(0.05f, 0.15f, 0.0f));  // D = 3
	points.push_back(glm::vec3(0.05f, 0.85f, 0.0f));  // E = 4
	points.push_back(glm::vec3(-0.05f, 0.85f, 0.0f)); // F = 5
	points.push_back(glm::vec3(-0.05f, 0.15f, 0.0f)); // G = 6
	points.push_back(glm::vec3(-0.5f, 0.15f, 0.0f));  // H = 7
	points.push_back(glm::vec3(0.0f, -0.15f, 0.0f));  // I = 8

	// pendulum model triangles
	modelVertices.push_back(points[0]);
	modelVertices.push_back(points[6]);
	modelVertices.push_back(points[7]);
	modelVertices.push_back(points[0]);
	modelVertices.push_back(points[8]);
	modelVertices.push_back(points[6]);
	modelVertices.push_back(points[8]);
	modelVertices.push_back(points[3]);
	modelVertices.push_back(points[6]);
	modelVertices.push_back(points[8]);
	modelVertices.push_back(points[1]);
	modelVertices.push_back(points[3]);
	modelVertices.push_back(points[1]);
	modelVertices.push_back(points[2]);
	modelVertices.push_back(points[3]);
	modelVertices.push_back(points[6]);
	modelVertices.push_back(points[3]);
	modelVertices.push_back(points[4]);
	modelVertices.push_back(points[6]);
	modelVertices.push_back(points[4]);
	modelVertices.push_back(points[5]);

	modelOriginOffsetData.push_back(glm::vec3(0.0f, -0.85f, 0.0f));

	// color
	glm::vec4 modelColor(1.0f, 1.0f, 0.0f, 1.0f);
	for (unsigned int i = 0; i < modelVertices.size(); i++)
		colorData.push_back(modelColor);

	gameState->physicalObjectRenderer->addPhysicalObject(this);
	shouldRender = true;
}

void PoPendulum::initPhysics() {

	glm::mat4 worldTransform;
	worldTransform = glm::translate(worldTransform, initialPosition + (modelOriginOffsetData[0] * sizeScaler));

	// hinge point
	glm::mat4 hingePointTransform;
	hingePointTransform = glm::translate(worldTransform, glm::vec3(0.0f, 0.85 * sizeScaler, 0.0f));
	hingePointMass = new PhysicalMass();
	hingePointMass->init(gameState, 0.0f, hingePointTransform, PhysicsManager::CollisionGroup::NO_COLLISION);
	hingePointMass->addToDynamicsWorld();

	// bob mass and collider shapes
	glm::mat4 identityTransform;
	glm::mat4 armComTransform = glm::translate(identityTransform, glm::vec3(0.0f, ((0.7f / 2.0f) + 0.15f) * sizeScaler, 0.0f));
	bobMass = new PhysicalMass();
	bobMass->init(gameState, 500.0f, worldTransform, PhysicsManager::CollisionGroup::SWINGING_MASS);
	bobMass->addCollisionShapeBox(armComTransform, glm::vec3(0.1f, 0.7f, 0.0f) * sizeScaler);
	bobMass->addCollisionShapeBox(identityTransform, glm::vec3(1.0f, 0.3f, 0.0f) * sizeScaler);
	bobMass->addToDynamicsWorld();

	// hinge constraint
	hingeConstraint = new btHingeConstraint(
		*(hingePointMass->rigidBody),
		*(bobMass->rigidBody),
		btVector3(0.0f, 0.0f, 0.0f),
		btVector3(0.0f, 0.85f * sizeScaler, 0.0f),
		btVector3(0.0f, 0.0f, 1.0f),
		btVector3(0.0f, 0.0f, 0.0f),
		true
	);
	hingeConstraint->enableAngularMotor(true, 0.0f, 1.20f);
	gameState->physicsManager->dynamicsWorld->addConstraint(hingeConstraint);

}

void PoPendulum::doPhysicalUpdate() {

	glm::vec3 pos;
	bobMass->getCenterOfMassPosition(pos);

	Event e;
	e.eventPoster = this;
	e.eventType = Event::EventType::GAME_EVENT;
	e.gameEvent = Event::GameEvent::MOVE_RETICULE;
	e.eventWorldCoordinateX = pos.x;
	e.eventWorldCoordinateY = pos.y;
	gameState->eventBus->postEvent(e);

}

void PoPendulum::doRenderUpdate() {

	transformData.clear();

	// model transform
	glm::mat4 modelTransform;
	modelTransform = glm::translate(modelTransform, initialPosition);
	modelTransform = glm::scale(modelTransform, glm::vec3(sizeScaler, sizeScaler, 1.0f));
	glm::quat rotationQuaternion;
	PhysicsManager::btQuatToGlmQuat(bobMass->rigidBody->getOrientation(), rotationQuaternion);
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
	gameState->physicsManager->dynamicsWorld->removeConstraint(hingeConstraint);
	delete hingePointMass;
	delete bobMass;
	delete hingeConstraint;
}