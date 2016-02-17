#include "PhysicsManager.hpp"
#include <iostream>

PhysicsManager::PhysicsManager(){

	collisionBroadphase = new btDbvtBroadphase();
	collisionConfiguration = new btDefaultCollisionConfiguration();
	collisionDispatcher = new btCollisionDispatcher(collisionConfiguration);
	collisionSolver = new btSequentialImpulseConstraintSolver;

	dynamicsWorld = new btDiscreteDynamicsWorld(collisionDispatcher, collisionBroadphase, collisionSolver, collisionConfiguration);
	dynamicsWorld->setGravity(btVector3(0.0f, -9.81f, 0.0f));


	// for test
	groundShape = new btStaticPlaneShape(btVector3(0.0f, 1.0f, 0.0f), 1.0f);
	groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f), btVector3(0.0f, -1.0f, 0.0f)));
	btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0.0f, groundMotionState, groundShape, btVector3(0.0f, 0.0f, 0.0f));
	groundRigidBody = new btRigidBody(groundRigidBodyCI);
	dynamicsWorld->addRigidBody(groundRigidBody);

	leftWallShape = new btStaticPlaneShape(btVector3(1.0f, 0.0f, 0.0f), 0.0f);
	leftWallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f), btVector3(0.0f, -1.0f, 0.0f)));
	btRigidBody::btRigidBodyConstructionInfo leftWallRigidBodyCI(0.0f, leftWallMotionState, leftWallShape, btVector3(0.0f, 0.0f, 0.0f));
	leftWallRigidBody = new btRigidBody(leftWallRigidBodyCI);
	dynamicsWorld->addRigidBody(leftWallRigidBody);

	rightWallShape = new btStaticPlaneShape(btVector3(-1.0f, 0.0f, 0.0f), -35.0f);
	rightWallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f), btVector3(0.0f, -1.0f, 0.0f)));
	btRigidBody::btRigidBodyConstructionInfo rightWallRigidBodyCI(0.0f, rightWallMotionState, rightWallShape, btVector3(0.0f, 0.0f, 0.0f));
	rightWallRigidBody = new btRigidBody(rightWallRigidBodyCI);
	dynamicsWorld->addRigidBody(rightWallRigidBody);

	ceilingShape = new btStaticPlaneShape(btVector3(0.0f, -1.0f, 0.0f), -21.0f);
	ceilingMotionState = new btDefaultMotionState(btTransform(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f), btVector3(0.0f, -1.0f, 0.0f)));
	btRigidBody::btRigidBodyConstructionInfo ceilingRigidBodyCI(0.0f, ceilingMotionState, ceilingShape, btVector3(0.0f, 0.0f, 0.0f));
	ceilingRigidBody = new btRigidBody(ceilingRigidBodyCI);
	dynamicsWorld->addRigidBody(ceilingRigidBody);

}

void PhysicsManager::setDebugRenderer(btIDebugDraw* debugRenderer) {
	dynamicsWorld->setDebugDrawer(debugRenderer);
	dynamicsWorld->getDebugDrawer()->setDebugMode(1);
}

void PhysicsManager::updatePhysics() {
	dynamicsWorld->stepSimulation(1 / 60.0f, 10);
	dynamicsWorld->debugDrawWorld();
}

PhysicsManager::~PhysicsManager() {

	// test
	dynamicsWorld->removeRigidBody(groundRigidBody);
	delete groundMotionState;
	delete groundRigidBody;
	delete groundShape;

	dynamicsWorld->removeRigidBody(leftWallRigidBody);
	delete leftWallMotionState;
	delete leftWallRigidBody;
	delete leftWallShape;
	
	dynamicsWorld->removeRigidBody(rightWallRigidBody);
	delete rightWallMotionState;
	delete rightWallRigidBody;
	delete rightWallShape;

	dynamicsWorld->removeRigidBody(ceilingRigidBody);
	delete ceilingMotionState;
	delete ceilingRigidBody;
	delete ceilingShape;

	delete dynamicsWorld;
	delete collisionSolver;
	delete collisionConfiguration;
	delete collisionDispatcher;
	delete collisionBroadphase;
	
}

void PhysicsManager::glmVec3ToBtVec3(const glm::vec3& glmVector, btVector3& btVector) {
	btVector.setValue(glmVector.x, glmVector.y, glmVector.z);
}

void PhysicsManager::btVec3ToGlmVec3(const btVector3& btVector, glm::vec3& glmVector) {
	glmVector.x = btVector.x();
	glmVector.y = btVector.y();
	glmVector.z = btVector.z();
}

void PhysicsManager::btTransformToGlmTransform(const btTransform& btTrans, glm::mat4& glmTrans) {
	btTrans.getOpenGLMatrix(glm::value_ptr(glmTrans));
}
