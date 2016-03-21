#include "PhysicsManager.hpp"
#include <iostream>


PhysicsManager::PhysicsManager(){

	// init collision group interactions
	collisionGroupInteractions[NO_COLLISION] = NO_COLLISION;
	collisionGroupInteractions[ROPE_MASS] = SWINGING_MASS;
	collisionGroupInteractions[PLAYER] = PLAYER | BOUNDARY | SWINGING_MASS;
	collisionGroupInteractions[BOUNDARY] = PLAYER | SWINGING_MASS;
	collisionGroupInteractions[SWINGING_MASS] = PLAYER | BOUNDARY | ROPE_MASS;

	//collisionBroadphase = new btDbvtBroadphase();
	//btAxisSweep3(const btVector3& worldAabbMin, const btVector3& worldAabbMax, unsigned short int maxHandles = 16384, btOverlappingPairCache* pairCache = 0, bool disableRaycastAccelerator = false);

	collisionBroadphase = new btAxisSweep3(btVector3(-250.0f, -250.0f, -10.0f), btVector3(250.0f, 250.0f, 10.0f));
	collisionConfiguration = new btDefaultCollisionConfiguration();
//	collisionConfiguration = new btSoftBodyRigidBodyCollisionConfiguration();

	collisionDispatcher = new btCollisionDispatcher(collisionConfiguration);
	constraintSolver = new btSequentialImpulseConstraintSolver();
	softBodyConstraintSolver = new btDefaultSoftBodySolver();
	
	dynamicsWorld = new btSoftRigidDynamicsWorld(collisionDispatcher, collisionBroadphase, constraintSolver, collisionConfiguration, softBodyConstraintSolver);
	dynamicsWorld->setGravity(btVector3(0.0f, -9.81f, 0.0f));
	//btContactSolverInfo& info = dynamicsWorld->getSolverInfo();
	//std::cout << "current solver iterations = " << info.m_numIterations << std::endl;
//	std::cout << "info.m_timeStep = " << info.m_timeStep << std::endl;
	//info.m_timeStep = 0.0005f;
	//std::cout << "info.m_splitImpulse = " << info.m_splitImpulse << std::endl;
	//info.m_timeStep .m_numIterations = 500;

	// Box2d
	box2dWorld = new b2World(b2Vec2(0.0f, -9.81f));

	// create a static body that joints can use as a refernce when wanting to constrain to the world
	b2BodyDef worldStaticBodyDef;
	worldStaticBody = box2dWorld->CreateBody(&worldStaticBodyDef);
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

void PhysicsManager::setBox2dDebugRenderer(b2Draw* debugRenderer) {

	debugRenderer->SetFlags(
		0
		+ b2Draw::e_shapeBit	///< draw shapes
		+ b2Draw::e_jointBit	///< draw joint connections
		+ b2Draw::e_aabbBit	///< draw axis aligned bounding boxes
		+ b2Draw::e_pairBit      	///< draw broad-phase pairs
		+ b2Draw::e_centerOfMassBit
	);
	box2dWorld->SetDebugDraw(debugRenderer);

}

void PhysicsManager::updatePhysics() {
	dynamicsWorld->stepSimulation(1 / 60.0f, 10);
	//dynamicsWorld->stepSimulation(1 / 30.0f, 10);

	dynamicsWorld->debugDrawWorld();
	box2dWorld->DrawDebugData();
	box2dWorld->Step(1 / 60.0f, 6, 10);
	//box2dWorld->Step(1 / 60.0f, 6, 20);
	//box2dWorld->Step(1 / 60.0f, 6, 1000);
}

PhysicsManager::~PhysicsManager() {
	
	box2dWorld->DestroyBody(worldStaticBody);

	delete dynamicsWorld;
	delete constraintSolver;
	delete softBodyConstraintSolver;
	delete collisionConfiguration;
	delete collisionDispatcher;
	delete collisionBroadphase;

	delete box2dWorld;
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