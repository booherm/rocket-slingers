#include "PhysicalMass.hpp"
//#include <iostream>

void PhysicalMass::init(GameState* gameState, float mass, const glm::vec3& initialWorldPosition, float collisionShpereRadius) {
	this->mass = mass;
	this->initialWorldPosition = initialWorldPosition;
	this->physicsManager = gameState->physicsManager;

	collisionShape = new btSphereShape(collisionShpereRadius);
	btVector3 inertia(0.0f, 0.0f, 0.0f);
	collisionShape->calculateLocalInertia(mass, inertia);
	
	btRigidBody::btRigidBodyConstructionInfo rigidBodyConstructionInfo(mass, this, collisionShape, inertia);
	rigidBody = new btRigidBody(rigidBodyConstructionInfo);

	physicsManager->dynamicsWorld->addRigidBody(rigidBody);
}

// called by dynamics world after update only for objects that moved
void PhysicalMass::setWorldTransform(const btTransform& worldTrans) {

	PhysicsManager::btVec3ToGlmVec3(worldTrans.getOrigin(), worldPosition);
	PhysicsManager::btTransformToGlmTransform(worldTrans, worldTransform);

}

void PhysicalMass::getWorldTransform(btTransform& worldTrans) const {

	btVector3 initialPosition;
	PhysicsManager::glmVec3ToBtVec3(initialWorldPosition, initialPosition);
	worldTrans = btTransform(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f), initialPosition);

}

PhysicalMass::~PhysicalMass() {

	if (rigidBody != nullptr) {
		physicsManager->dynamicsWorld->removeRigidBody(rigidBody);
		delete rigidBody;
		delete collisionShape;
	}

}
