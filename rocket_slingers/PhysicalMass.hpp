#ifndef PHYSICALMASS_HPP
#define PHYSICALMASS_HPP

#include <glm.hpp>
#include <vector>
#include "PhysicsManager.hpp"
#include "GameState.hpp"

class PhysicalMass : public btMotionState {
public:

	void init(GameState* gameState, float mass, const glm::vec3& initialWorldPosition, float collisionShpereRadius);
	void setWorldTransform(const btTransform& worldTrans); 
	void getWorldTransform(btTransform& worldTrans) const;

	float mass;
	btRigidBody* rigidBody = nullptr;
	glm::vec3 initialWorldPosition;
	glm::vec3 worldPosition;
	glm::mat4 worldTransform;

	~PhysicalMass();

private:

	PhysicsManager* physicsManager;
	btCollisionShape* collisionShape = nullptr;

};

#endif
