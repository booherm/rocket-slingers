#include "PhysicalMass.hpp"
//#include <iostream>

void PhysicalMass::init(GameState* gameState, float mass, const glm::mat4& worldTransform, PhysicsManager::CollisionGroup collisionGroup) {
	this->physicsManager = gameState->physicsManager;
	this->mass = mass;
	this->worldTransform = worldTransform;
	this->collisionGroup = collisionGroup;

	collisionShape = new btCompoundShape();
}

void PhysicalMass::addCollisionShapeSphere(const glm::mat4& worldTransform, float collisionShpereRadius) {

	btCollisionShape* sphereCollisionShape = new btSphereShape(collisionShpereRadius);
	addCollisionShape(worldTransform, sphereCollisionShape);
	
}

void PhysicalMass::addCollisionShapeBox(const glm::mat4& worldTransform, const glm::vec3& boxExtents) {

	btVector3 boxHalfExtents;
	PhysicsManager::glmVec3ToBtVec3(boxExtents * 0.5f, boxHalfExtents);
	btCollisionShape* boxCollisionShape = new btBoxShape(boxHalfExtents);
	addCollisionShape(worldTransform, boxCollisionShape);

}

void PhysicalMass::addCollisionShape(const glm::mat4& worldTransform, btCollisionShape* collisionShape) {

	collisionShapeComponents.push_back(collisionShape);
	//this->collisionShape->addChildShape(btTransform::getIdentity(), collisionShape);
	
	btTransform worldTransformBt;
	PhysicsManager::glmTransformToBtTransform(worldTransform, worldTransformBt);
	this->collisionShape->addChildShape(worldTransformBt, collisionShape);

}

void PhysicalMass::addToDynamicsWorld() {

	btVector3 inertia(0.0f, 0.0f, 0.0f);
	collisionShape->calculateLocalInertia(mass, inertia);

	btRigidBody::btRigidBodyConstructionInfo rigidBodyConstructionInfo(mass, this, collisionShape, inertia);
	rigidBody = new btRigidBody(rigidBodyConstructionInfo);
//	physicsManager->dynamicsWorld->addRigidBody(rigidBody);
	physicsManager->dynamicsWorld->addRigidBody(rigidBody, collisionGroup, physicsManager->getCollisionGroupInteractions(collisionGroup));
	
}

void PhysicalMass::getCenterOfMassPosition(glm::vec3& position) {
	btVector3 something = rigidBody->getWorldTransform().getOrigin();
	PhysicsManager::btVec3ToGlmVec3(something, position);
}

// called by dynamics world after update only for objects that moved
void PhysicalMass::setWorldTransform(const btTransform& worldTrans) {

	PhysicsManager::btTransformToGlmTransform(worldTrans, worldTransform);

}

void PhysicalMass::getWorldTransform(btTransform& worldTrans) const {

	PhysicsManager::glmTransformToBtTransform(worldTransform, worldTrans);

}

PhysicalMass::~PhysicalMass() {

	for (unsigned int i = 0; i < collisionShapeComponents.size(); ++i) {
		delete collisionShapeComponents[i];
	}

	if (rigidBody != nullptr) {
		physicsManager->dynamicsWorld->removeRigidBody(rigidBody);
		delete rigidBody;
	}

	if (collisionShape != nullptr) {
		delete collisionShape;
	}

}
