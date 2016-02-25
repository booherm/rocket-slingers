#include "PhysicalMassMultiBody.hpp"
//#include <iostream>

void PhysicalMassMultiBody::init(GameState* gameState, float baseMass, const glm::mat4& baseWorldTransform){
	this->physicsManager = gameState->physicsManager;
	this->baseWorldTransform = baseWorldTransform;
	this->baseMass = baseMass;
}

void PhysicalMassMultiBody::pushMass(float mass, const glm::vec3& offsetFromParent, bool collidable, float collisionSphereRadius, int collisionGroupId, int collisionMask) {
	linkedMasses.push_back({ mass, offsetFromParent, collidable, collisionSphereRadius, collisionGroupId, collisionMask });
}

void PhysicalMassMultiBody::addToDynamicsWorld() {

	// setup multibody mass
	int linkedMassCount = linkedMasses.size();
	multiBody = new btMultiBody(linkedMassCount, baseMass, btVector3(0.0f, 0.0f, 0.0f), baseMass == 0, false);
	btTransform baseWorldTransformBt;
	PhysicsManager::glmTransformToBtTransform(baseWorldTransform, baseWorldTransformBt);
	multiBody->setBaseWorldTransform(baseWorldTransformBt);

	// setup linked masses
	for (int i = 0; i < linkedMassCount; ++i) {
		LinkedMass lm = linkedMasses[i];
		btVector3 parentCenterOfMassToThisCenterOfMassOffset;
		PhysicsManager::glmVec3ToBtVec3(lm.offsetFromParent, parentCenterOfMassToThisCenterOfMassOffset);
		btVector3 linkInertia(0.0f, 0.0f, 0.0f);
		btCollisionShape* sphereShape = new btSphereShape(lm.collisionSphereRadius);
		sphereShape->calculateLocalInertia(lm.mass, linkInertia);
		delete sphereShape;

		multiBody->setupRevolute(
			i,                                          // mass index
			lm.mass,                                    // new mass body mass
			linkInertia,                                // local inertia
			i - 1,                                      // parent mass index
			btQuaternion(0.0f, 0.0f, 0.0f, 1.0f),       // rotation to parent
			btVector3(0.0f, 0.0f, 1.0f),                // axis of rotation
			btVector3(0.0f, 0.0f, 0.0f),                // parent center of mass to current pivot offset - we treat the parent center of mass as the pivot point
			parentCenterOfMassToThisCenterOfMassOffset  // vector between parent mass body center of mass and this mass body center of mass
		);
	}

	// finalize multibody and add to world
	multiBody->finalizeMultiDof();
	physicsManager->dynamicsWorld->addMultiBody(multiBody);

	// debug - all needed?
	multiBody->setCanSleep(false);
	multiBody->setHasSelfCollision(true);
	multiBody->setUseGyroTerm(false);
	multiBody->setLinearDamping(0.1f);
	multiBody->setAngularDamping(0.9f);

	// setup collision shapes
	for (int i = 0; i < linkedMassCount; ++i)
	{
		if (linkedMasses[i].collidable) {
			btCollisionShape* collisionShape = new btSphereShape(linkedMasses[i].collisionSphereRadius);
			btMultiBodyLinkCollider* multiBodyLinkCollider = new btMultiBodyLinkCollider(multiBody, i);
			multiBodyLinkCollider->setCollisionShape(collisionShape);

			physicsManager->dynamicsWorld->addCollisionObject(multiBodyLinkCollider, linkedMasses[i].collisionGroupId, linkedMasses[i].collisionMask);
			multiBody->getLink(i).m_collider = multiBodyLinkCollider;

			linkedMasses[i].collisionShape = collisionShape;
			linkedMasses[i].multiBodyLinkCollider = multiBodyLinkCollider;
		}
	}

}

void PhysicalMassMultiBody::getBasePosition(glm::vec3& position) {
	btVector3 positionBt = multiBody->getBasePos();
	PhysicsManager::btVec3ToGlmVec3(positionBt, position);
}

void PhysicalMassMultiBody::setBasePosition(const glm::vec3& position) {
	btVector3 positionBt;
	PhysicsManager::glmVec3ToBtVec3(position, positionBt);
	multiBody->setBasePos(positionBt);
}

float PhysicalMassMultiBody::getPivotAngle(unsigned int pivotIndex) {
	return multiBody->getJointPos(pivotIndex);
}

void PhysicalMassMultiBody::getPivotPosition(unsigned int pivotIndex, glm::vec3& position) {
	btVector3 positionBt = multiBody->getLink(pivotIndex).m_cachedWorldTransform.getOrigin();
	PhysicsManager::btVec3ToGlmVec3(positionBt, position);
}

void PhysicalMassMultiBody::applyForceToBase(const glm::vec3& force) {
	btVector3 forceBt;
	PhysicsManager::glmVec3ToBtVec3(force, forceBt);
	multiBody->addBaseForce(forceBt);
}

void PhysicalMassMultiBody::applyForceToPivot(unsigned int pivotIndex, const glm::vec3& force) {
	btVector3 forceBt;
	PhysicsManager::glmVec3ToBtVec3(force, forceBt);
	multiBody->addLinkForce(pivotIndex, forceBt);
}

PhysicalMassMultiBody::~PhysicalMassMultiBody() {

	for (unsigned int i = 0; i < linkedMasses.size(); ++i) {
		if (linkedMasses[i].collidable) {
			btMultiBodyLinkCollider* multiBodyLinkCollider = linkedMasses[i].multiBodyLinkCollider;
			//physicsManager->dynamicsWorld->removeCollisionObject(multiBodyLinkCollider);
			physicsManager->dynamicsWorld->getCollisionWorld()->removeCollisionObject(multiBodyLinkCollider);
			delete linkedMasses[i].collisionShape;
			delete multiBodyLinkCollider;
		}
	}

	if (multiBody != nullptr) {
		physicsManager->dynamicsWorld->removeMultiBody(multiBody);
		delete multiBody;
	}


}
