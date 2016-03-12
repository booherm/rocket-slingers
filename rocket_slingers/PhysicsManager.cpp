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
	//collisionConfiguration = new btDefaultCollisionConfiguration();
	collisionConfiguration = new btSoftBodyRigidBodyCollisionConfiguration();
	collisionDispatcher = new btCollisionDispatcher(collisionConfiguration);
	constraintSolver = new btSequentialImpulseConstraintSolver();
	softBodyConstraintSolver = new btDefaultSoftBodySolver();
	//constraintSolver = new btMultiBodyConstraintSolver;

	//dynamicsWorld = new btDiscreteDynamicsWorld(collisionDispatcher, collisionBroadphase, constraintSolver, collisionConfiguration);
	//dynamicsWorld = new btMultiBodyDynamicsWorld(collisionDispatcher, collisionBroadphase, constraintSolver, collisionConfiguration);
	dynamicsWorld = new btSoftRigidDynamicsWorld(collisionDispatcher, collisionBroadphase, constraintSolver, collisionConfiguration, softBodyConstraintSolver);
	dynamicsWorld->setGravity(btVector3(0.0f, -9.81f, 0.0f));
	//dynamicsWorld->setGravity(btVector3(0.0f, 0.0f, 0.0f));

	// for test
	boundariesOn = false;
	if (boundariesOn) {

		groundShape = new btStaticPlaneShape(btVector3(0.0f, 1.0f, 0.0f), 0.0f);
		groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f), btVector3(0.0f, -1.0f, 0.0f)));
		btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0.0f, groundMotionState, groundShape, btVector3(0.0f, 0.0f, 0.0f));
		groundRigidBody = new btRigidBody(groundRigidBodyCI);
		groundRigidBody->setUserPointer(nullptr);
		dynamicsWorld->addRigidBody(groundRigidBody, BOUNDARY, collisionGroupInteractions[BOUNDARY]);

		leftWallShape = new btStaticPlaneShape(btVector3(1.0f, 0.0f, 0.0f), 0.0f);
		leftWallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f), btVector3(0.0f, -1.0f, 0.0f)));
		btRigidBody::btRigidBodyConstructionInfo leftWallRigidBodyCI(0.0f, leftWallMotionState, leftWallShape, btVector3(0.0f, 0.0f, 0.0f));
		leftWallRigidBody = new btRigidBody(leftWallRigidBodyCI);
		leftWallRigidBody->setUserPointer(nullptr);
		dynamicsWorld->addRigidBody(leftWallRigidBody, BOUNDARY, collisionGroupInteractions[BOUNDARY]);

		rightWallShape = new btStaticPlaneShape(btVector3(-1.0f, 0.0f, 0.0f), -55.5f * 5.0f);
		rightWallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f), btVector3(0.0f, -1.0f, 0.0f)));
		btRigidBody::btRigidBodyConstructionInfo rightWallRigidBodyCI(0.0f, rightWallMotionState, rightWallShape, btVector3(0.0f, 0.0f, 0.0f));
		rightWallRigidBody = new btRigidBody(rightWallRigidBodyCI);
		rightWallRigidBody->setUserPointer(nullptr);
		dynamicsWorld->addRigidBody(rightWallRigidBody, BOUNDARY, collisionGroupInteractions[BOUNDARY]);

		ceilingShape = new btStaticPlaneShape(btVector3(0.0f, -1.0f, 0.0f), -20.0f * 5.0f);
		ceilingMotionState = new btDefaultMotionState(btTransform(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f), btVector3(0.0f, -1.0f, 0.0f)));
		btRigidBody::btRigidBodyConstructionInfo ceilingRigidBodyCI(0.0f, ceilingMotionState, ceilingShape, btVector3(0.0f, 0.0f, 0.0f));
		ceilingRigidBody = new btRigidBody(ceilingRigidBodyCI);
		ceilingRigidBody->setUserPointer(nullptr);
		dynamicsWorld->addRigidBody(ceilingRigidBody, BOUNDARY, collisionGroupInteractions[BOUNDARY]);


		/*
		boxShape = new btBoxShape(btVector3(1.0f, 1.0f, 0.0f));
		boxMotionState = new btDefaultMotionState();
		btRigidBody::btRigidBodyConstructionInfo boxCI(10.0f, boxMotionState, boxShape, btVector3(0.0f, 0.0f, 0.0f));
		boxRigidBody = new btRigidBody(boxCI);
		boxRigidBody->setUserPointer(nullptr);
		btTransform boxTransform;
		boxTransform.setIdentity();
		boxTransform.setOrigin(btVector3(20.0f, 20.0f, 0.0f));
		boxRigidBody->setWorldTransform(boxTransform);
		dynamicsWorld->addRigidBody(boxRigidBody, SWINGING_MASS, collisionGroupInteractions[SWINGING_MASS]);
		*/
	}
}

unsigned int PhysicsManager::getCollisionGroupInteractions(CollisionGroup collisionGroup) {
	return collisionGroupInteractions[collisionGroup];
}

bool PhysicsManager::testRayHit(const glm::vec3& fromPoint, const glm::vec3& toPoint, CollisionGroup collisionGroup, glm::vec3& hitLocation) {

	btVector3 fromPointBt, toPointBt;
	glmVec3ToBtVec3(fromPoint, fromPointBt);
	glmVec3ToBtVec3(toPoint, toPointBt);

	btCollisionWorld::ClosestRayResultCallback hitResult(fromPointBt, toPointBt);
	hitResult.m_collisionFilterGroup = collisionGroup;
	hitResult.m_collisionFilterMask = collisionGroupInteractions[collisionGroup];
	dynamicsWorld->rayTest(fromPointBt, toPointBt, hitResult);

	if (hitResult.hasHit()) {
		PhysicsManager::btVec3ToGlmVec3(hitResult.m_hitPointWorld, hitLocation);
		return true;
	}

	return false;
}

void PhysicsManager::setDebugRenderer(btIDebugDraw* debugRenderer) {
	dynamicsWorld->setDebugDrawer(debugRenderer);
	dynamicsWorld->getDebugDrawer()->setDebugMode(
		0
//		+ btIDebugDraw::DBG_DrawConstraints
		+ btIDebugDraw::DBG_DrawWireframe
//		+ btIDebugDraw::DBG_DrawContactPoints
		+ btIDebugDraw::DBG_DrawAabb
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

	if (boundariesOn)
	{
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

		/*
		dynamicsWorld->removeRigidBody(boxRigidBody);
		delete boxMotionState;
		delete boxRigidBody;
		delete boxShape;
		*/

	}

	delete dynamicsWorld;
	delete constraintSolver;
	delete softBodyConstraintSolver;
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