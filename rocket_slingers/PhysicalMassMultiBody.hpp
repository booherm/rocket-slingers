#ifndef PHYSICALMASSMULTIBODY_HPP
#define PHYSICALMASSMULTIBODY_HPP
/*
#include <glm.hpp>
#include <vector>
#include "PhysicsManager.hpp"
#include "GameState.hpp"

ATTRIBUTE_ALIGNED16(class) PhysicalMassMultiBody {
public:

	BT_DECLARE_ALIGNED_ALLOCATOR();
	void init(GameState* gameState, float baseMass, const glm::mat4& baseWorldTransform);
	void pushMass(float mass, const glm::vec3& offsetFromParent, bool collidable, float collisionSphereRadius, int collisionGroupId, int collisionMask);
	void addToDynamicsWorld();
	void setBasePosition(const glm::vec3& position);
	void getBasePosition(glm::vec3& position);
	float getPivotAngle(unsigned int pivotIndex);
	void getPivotPosition(unsigned int pivotIndex, glm::vec3& position);
	void applyForceToBase(const glm::vec3& force);
	void applyForceToPivot(unsigned int pivotIndex, const glm::vec3& force);
	~PhysicalMassMultiBody();
	btMultiBody* multiBody = nullptr;

	glm::mat4 baseWorldTransform;

private:

	struct LinkedMass {
		float mass;
		glm::vec3 offsetFromParent;
		bool collidable;
		float collisionSphereRadius;
		int collisionGroupId;
		int collisionMask;
		btCollisionShape* collisionShape;
		btMultiBodyLinkCollider* multiBodyLinkCollider;
	};

	PhysicsManager* physicsManager = nullptr;
	float baseMass;
	btVector3 baseMassBodyCenterOfMass;
	std::vector<LinkedMass> linkedMasses;

};
*/
#endif
