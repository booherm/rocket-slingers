#include "PhysicsManager.hpp"
#include <iostream>

PhysicsManager::PhysicsManager(){

	// init collision group interactions
	collisionGroupInteractions[NO_COLLISION] = NO_COLLISION;
	collisionGroupInteractions[ROPE_MASS] = SWINGING_MASS;
	collisionGroupInteractions[PLAYER] = PLAYER | BOUNDARY | SWINGING_MASS;
	collisionGroupInteractions[BOUNDARY] = PLAYER | SWINGING_MASS;
	collisionGroupInteractions[SWINGING_MASS] = PLAYER | BOUNDARY | ROPE_MASS;

	collisionBroadphase = new btDbvtBroadphase();
	collisionConfiguration = new btDefaultCollisionConfiguration();
	collisionDispatcher = new btCollisionDispatcher(collisionConfiguration);
	//constraintSolver = new btSequentialImpulseConstraintSolver;
	constraintSolver = new btMultiBodyConstraintSolver;

	//dynamicsWorld = new btDiscreteDynamicsWorld(collisionDispatcher, collisionBroadphase, constraintSolver, collisionConfiguration);
	dynamicsWorld = new btMultiBodyDynamicsWorld(collisionDispatcher, collisionBroadphase, constraintSolver, collisionConfiguration);
	dynamicsWorld->setGravity(btVector3(0.0f, -9.81f, 0.0f));
	//dynamicsWorld->setGravity(btVector3(0.0f, 0.0f, 0.0f));


	// for test
	groundShape = new btStaticPlaneShape(btVector3(0.0f, 1.0f, 0.0f), 0.0f);
	groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f), btVector3(0.0f, -1.0f, 0.0f)));
	btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0.0f, groundMotionState, groundShape, btVector3(0.0f, 0.0f, 0.0f));
	groundRigidBody = new btRigidBody(groundRigidBodyCI);
	dynamicsWorld->addRigidBody(groundRigidBody, BOUNDARY, collisionGroupInteractions[BOUNDARY]);

	leftWallShape = new btStaticPlaneShape(btVector3(1.0f, 0.0f, 0.0f), 0.0f);
	leftWallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f), btVector3(0.0f, -1.0f, 0.0f)));
	btRigidBody::btRigidBodyConstructionInfo leftWallRigidBodyCI(0.0f, leftWallMotionState, leftWallShape, btVector3(0.0f, 0.0f, 0.0f));
	leftWallRigidBody = new btRigidBody(leftWallRigidBodyCI);
	dynamicsWorld->addRigidBody(leftWallRigidBody, BOUNDARY, collisionGroupInteractions[BOUNDARY]);

	rightWallShape = new btStaticPlaneShape(btVector3(-1.0f, 0.0f, 0.0f), -35.5f * 5.0f);
	rightWallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f), btVector3(0.0f, -1.0f, 0.0f)));
	btRigidBody::btRigidBodyConstructionInfo rightWallRigidBodyCI(0.0f, rightWallMotionState, rightWallShape, btVector3(0.0f, 0.0f, 0.0f));
	rightWallRigidBody = new btRigidBody(rightWallRigidBodyCI);
	dynamicsWorld->addRigidBody(rightWallRigidBody, BOUNDARY, collisionGroupInteractions[BOUNDARY]);

	ceilingShape = new btStaticPlaneShape(btVector3(0.0f, -1.0f, 0.0f), -20.0f * 5.0f);
	ceilingMotionState = new btDefaultMotionState(btTransform(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f), btVector3(0.0f, -1.0f, 0.0f)));
	btRigidBody::btRigidBodyConstructionInfo ceilingRigidBodyCI(0.0f, ceilingMotionState, ceilingShape, btVector3(0.0f, 0.0f, 0.0f));
	ceilingRigidBody = new btRigidBody(ceilingRigidBodyCI);
	dynamicsWorld->addRigidBody(ceilingRigidBody, BOUNDARY, collisionGroupInteractions[BOUNDARY]);

}

unsigned int PhysicsManager::getCollisionGroupInteractions(CollisionGroup collisionGroup) {
	return collisionGroupInteractions[collisionGroup];
}

void PhysicsManager::setDebugRenderer(btIDebugDraw* debugRenderer) {
	dynamicsWorld->setDebugDrawer(debugRenderer);
//	dynamicsWorld->getDebugDrawer()->setDebugMode(1);

	dynamicsWorld->getDebugDrawer()->setDebugMode(
		0
//		+ btIDebugDraw::DBG_DrawConstraints
		+ btIDebugDraw::DBG_DrawWireframe
//		+ btIDebugDraw::DBG_DrawContactPoints
//		+ btIDebugDraw::DBG_DrawAabb
//		+ btIDebugDraw::DBG_DrawConstraintLimits
//		+ btIDebugDraw::DBG_DrawFeaturesText
//		+ btIDebugDraw::DBG_DrawFrames
//		+ btIDebugDraw::DBG_DrawNormals
//		+ btIDebugDraw::DBG_DrawText
	);
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
	delete constraintSolver;
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

void PhysicsManager::glmTransformToBtTransform(const glm::mat4& glmTrans, btTransform& btTrans) {
	btTrans.setFromOpenGLMatrix(glm::value_ptr(glmTrans));
}

void PhysicsManager::btTransformToGlmTransform(const btTransform& btTrans, glm::mat4& glmTrans) {
	btTrans.getOpenGLMatrix(glm::value_ptr(glmTrans));
}

void PhysicsManager::btQuatToGlmQuat(const btQuaternion& btQuat, glm::quat& glmQuat) {
	btVector3 axis = btQuat.getAxis();
	glmQuat = glm::angleAxis(btQuat.getAngle(), glm::vec3(axis.x(), axis.y(), axis.z()));
}