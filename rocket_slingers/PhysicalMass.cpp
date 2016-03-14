#include "PhysicalMass.hpp"

std::map<std::string, PhysicalMass*> PhysicalMass::allPhysicalMasses;

void PhysicalMass::init(const std::string& id, GameState* gameState, float mass, const glm::mat4& worldTransform, PhysicsManager::CollisionGroup collisionGroup) {
	this->id = id;
	this->physicsManager = gameState->physicsManager;
	this->mass = mass;
	this->worldTransform = worldTransform;
	this->collisionGroup = collisionGroup;

	ccdEnabled = false;
	allPhysicalMasses[id] = this;
	collisionShape = new btCompoundShape(false, 0);
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

void PhysicalMass::enableCcd() {
	ccdEnabled = true;
}

void PhysicalMass::addCollisionShape(const glm::mat4& worldTransform, btCollisionShape* collisionShape) {

	collisionShapeComponents.push_back(collisionShape);
	//this->collisionShape = collisionShape;
	btTransform worldTransformBt;
	PhysicsManager::glmTransformToBtTransform(worldTransform, worldTransformBt);
	this->collisionShape->addChildShape(worldTransformBt, collisionShape);

}

void PhysicalMass::addToDynamicsWorld() {
	
	btVector3 inertia(0.0f, 0.0f, 0.0f);
	collisionShape->calculateLocalInertia(mass, inertia);

	btRigidBody::btRigidBodyConstructionInfo rigidBodyConstructionInfo(mass, this, collisionShape, inertia);
	rigidBody = new btRigidBody(rigidBodyConstructionInfo);
	rigidBody->setUserPointer(this);

	if (ccdEnabled) {
		rigidBody->setCcdMotionThreshold(1.0f);
		rigidBody->setCcdSweptSphereRadius(5.0f);
	}

	physicsManager->dynamicsWorld->addRigidBody(rigidBody, collisionGroup, physicsManager->getCollisionGroupInteractions(collisionGroup));

}

void PhysicalMass::getCenterOfMassPosition(glm::vec3& position) {
	btVector3 com = rigidBody->getWorldTransform().getOrigin();
	PhysicsManager::btVec3ToGlmVec3(com, position);
}

// called by dynamics world after update only for objects that moved
void PhysicalMass::setWorldTransform(const btTransform& worldTrans) {

	PhysicsManager::btTransformToGlmTransform(worldTrans, worldTransform);

}

void PhysicalMass::getWorldTransform(btTransform& worldTrans) const {

	PhysicsManager::glmTransformToBtTransform(worldTransform, worldTrans);

}

void PhysicalMass::setGravity(const glm::vec3& gravity) {
	btVector3 gravityBt;
	PhysicsManager::glmVec3ToBtVec3(gravity, gravityBt);
	rigidBody->setGravity(gravityBt);
}

void PhysicalMass::setActivationState(int state) {
	rigidBody->setActivationState(state);
}

/*
void PhysicalMass::setCenterOfMassTransform(const glm::mat4& transform) {
	btTransform transformBt;
	PhysicsManager::glmTransformToBtTransform(transform, transformBt);
	rigidBody->setCenterOfMassTransform(transformBt);
}
*/

void PhysicalMass::applyCentralImpulse(const glm::vec3& impulse) {
	btVector3 impulseBt;
	PhysicsManager::glmVec3ToBtVec3(impulse, impulseBt);
	rigidBody->applyCentralImpulse(impulseBt);
}


PhysicalMass::~PhysicalMass() {

	allPhysicalMasses.erase(id);

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
