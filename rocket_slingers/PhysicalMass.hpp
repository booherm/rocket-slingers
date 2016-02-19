#ifndef PHYSICALMASS_HPP
#define PHYSICALMASS_HPP

#include <glm.hpp>
#include <vector>
#include "PhysicsManager.hpp"
#include "GameState.hpp"

class PhysicalMass : public btMotionState {
public:

	void init(GameState* gameState, float mass, const glm::mat4& worldTransform);
	void addCollisionShapeSphere(const glm::mat4& worldTransform, float collisionShpereRadius);
	void addCollisionShapeBox(const glm::mat4& worldTransform, const glm::vec3& boxExtents);
	void addToDynamicsWorld();

	// try private:
	void setWorldTransform(const btTransform& worldTrans);
	void getWorldTransform(btTransform& worldTrans) const;

	float mass;
	btRigidBody* rigidBody = nullptr;
	glm::mat4 worldTransform;

	~PhysicalMass();

private:

	PhysicsManager* physicsManager;
	btCompoundShape* collisionShape = nullptr;
	std::vector<btCollisionShape*> collisionShapeComponents;

	void addCollisionShape(const glm::mat4& transform, btCollisionShape* collisionShape);

};

#endif
