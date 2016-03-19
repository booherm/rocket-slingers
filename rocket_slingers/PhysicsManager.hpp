#ifndef PHYSICSMANAGER_HPP
#define PHYSICSMANAGER_HPP

#include <btBulletDynamicsCommon.h>
#include <BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h>
#include <BulletSoftBody/btDefaultSoftBodySolver.h>
#include <BulletSoftBody/btSoftRigidDynamicsWorld.h>

#include <Box2D/Box2D.h>

#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <map>

class PhysicsManager {
public:

	enum CollisionGroup {
		NO_COLLISION  = 1,
		ROPE_MASS     = 2,
		PLAYER        = 4,
		BOUNDARY      = 8,
		SWINGING_MASS = 16
	};

	PhysicsManager();
	void setDebugRenderer(btIDebugDraw* debugRenderer);
	void setBox2dDebugRenderer(b2Draw* debugRenderer);
	void updatePhysics();
	unsigned int getCollisionGroupInteractions(CollisionGroup collisionGroup);
	bool testRayHit(const glm::vec3& fromPoint, const glm::vec3& toPoint, CollisionGroup collisionGroup, glm::vec3& hitLocation);
	~PhysicsManager();

	//btDiscreteDynamicsWorld* dynamicsWorld;
	btSoftRigidDynamicsWorld* dynamicsWorld;

	static void glmVec3ToBtVec3(const glm::vec3& glmVector, btVector3& btVector);
	static void btVec3ToGlmVec3(const btVector3& btVector, glm::vec3& glmVector);
	static void glmTransformToBtTransform(const glm::mat4& glmTrans, btTransform& btTrans);
	static void btTransformToGlmTransform(const btTransform& btTrans, glm::mat4& glmTrans);
	static void btQuatToGlmQuat(const btQuaternion& btQuat, glm::quat& glmQuat);


	// Box2D
	b2World* box2dWorld;



private:

	std::map<CollisionGroup, unsigned int> collisionGroupInteractions;

	btBroadphaseInterface* collisionBroadphase;
	btDefaultCollisionConfiguration* collisionConfiguration;
	//btSoftBodyRigidBodyCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* collisionDispatcher;
	btSequentialImpulseConstraintSolver* constraintSolver;
	btDefaultSoftBodySolver* softBodyConstraintSolver;
};

#endif
