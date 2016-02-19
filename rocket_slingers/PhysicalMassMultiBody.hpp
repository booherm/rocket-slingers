#ifndef PHYSICALMASSMULTIBODY_HPP
#define PHYSICALMASSMULTIBODY_HPP

#include <glm.hpp>
#include <vector>
#include "PhysicsManager.hpp"
#include "GameState.hpp"

ATTRIBUTE_ALIGNED16(class) PhysicalMassMultiBody {
public:

	BT_DECLARE_ALIGNED_ALLOCATOR();
	void init(GameState* gameState, float baseMass, const glm::mat4& baseWorldTransform);
	void pushMass(float mass, const glm::vec3& offsetFromParent);
	void addToDynamicsWorld();
	float getPivotPosition(unsigned int pivotIndex);
	~PhysicalMassMultiBody();

	glm::mat4 baseWorldTransform;

private:

	struct LinkedMass {
		float mass;
		glm::vec3 offsetFromParent;
		btCollisionShape* collisionShape;
		btMultiBodyLinkCollider* multiBodyLinkCollider;
	};

	PhysicsManager* physicsManager = nullptr;
	float baseMass;
	btVector3 baseMassBodyCenterOfMass;
	btMultiBody* multiBody = nullptr;
	std::vector<LinkedMass> linkedMasses;

};

#endif
