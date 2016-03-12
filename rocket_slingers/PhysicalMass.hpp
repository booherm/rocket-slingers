#ifndef PHYSICALMASS_HPP
#define PHYSICALMASS_HPP

#include <glm.hpp>
#include <vector>
#include "PhysicsManager.hpp"
#include "GameState.hpp"

class PhysicalMass : public btMotionState {
public:

	void init(const std::string& id, GameState* gameState, float mass, const glm::mat4& worldTransform, PhysicsManager::CollisionGroup collisionGroup);
	void addCollisionShapeSphere(const glm::mat4& worldTransform, float collisionShpereRadius);
	void addCollisionShapeBox(const glm::mat4& worldTransform, const glm::vec3& boxExtents);
	void addToDynamicsWorld();
	void applyCentralImpulse(const glm::vec3& impulse);
	void getCenterOfMassPosition(glm::vec3& position);
	//void setCenterOfMassTransform(const glm::mat4& transform);
	void setGravity(const glm::vec3& gravity);
	void setActivationState(int state);

	// try private:
	void setWorldTransform(const btTransform& worldTrans);
	void getWorldTransform(btTransform& worldTrans) const;

	float mass;
	std::string id;
	btRigidBody* rigidBody = nullptr;
	glm::mat4 worldTransform;
	~PhysicalMass();

private:

	static std::map<std::string, PhysicalMass*> allPhysicalMasses;

	PhysicsManager* physicsManager;
	btCompoundShape* collisionShape = nullptr;
	//btCollisionShape* collisionShape = nullptr;
	std::vector<btCollisionShape*> collisionShapeComponents;
	PhysicsManager::CollisionGroup collisionGroup;

	void addCollisionShape(const glm::mat4& transform, btCollisionShape* collisionShape);

};

#endif
