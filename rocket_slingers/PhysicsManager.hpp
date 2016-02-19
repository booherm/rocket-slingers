#ifndef PHYSICSMANAGER_HPP
#define PHYSICSMANAGER_HPP

#include <btBulletDynamicsCommon.h>
#include <BulletDynamics/Featherstone/btMultiBodyDynamicsWorld.h>
#include <BulletDynamics/Featherstone/btMultiBodyConstraintSolver.h>
#include <BulletDynamics/Featherstone/btMultiBodyLinkCollider.h>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>

class PhysicsManager {
public:
	PhysicsManager();
	void setDebugRenderer(btIDebugDraw* debugRenderer);
	void updatePhysics();
	~PhysicsManager();

	//btDiscreteDynamicsWorld* dynamicsWorld;
	btMultiBodyDynamicsWorld*  dynamicsWorld;

	static void glmVec3ToBtVec3(const glm::vec3& glmVector, btVector3& btVector);
	static void btVec3ToGlmVec3(const btVector3& btVector, glm::vec3& glmVector);
	static void glmTransformToBtTransform(const glm::mat4& glmTrans, btTransform& btTrans);
	static void btTransformToGlmTransform(const btTransform& btTrans, glm::mat4& glmTrans);

private:

	btBroadphaseInterface* collisionBroadphase;
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* collisionDispatcher;
	//btSequentialImpulseConstraintSolver* constraintSolver;
	btMultiBodyConstraintSolver* constraintSolver;



	btStaticPlaneShape* groundShape;
	btDefaultMotionState* groundMotionState;
	btRigidBody* groundRigidBody;

	btStaticPlaneShape* leftWallShape;
	btDefaultMotionState* leftWallMotionState;
	btRigidBody* leftWallRigidBody;

	btStaticPlaneShape* rightWallShape;
	btDefaultMotionState* rightWallMotionState;
	btRigidBody* rightWallRigidBody;

	btStaticPlaneShape* ceilingShape;
	btDefaultMotionState* ceilingMotionState;
	btRigidBody* ceilingRigidBody;

};

#endif
