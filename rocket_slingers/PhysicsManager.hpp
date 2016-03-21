#ifndef PHYSICSMANAGER_HPP
#define PHYSICSMANAGER_HPP

#include <Box2D/Box2D.h>

#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <map>
#include "WorldRayCastCallback.hpp"

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
	void setBox2dDebugRenderer(b2Draw* debugRenderer);
	void updatePhysics();
	unsigned int getCollisionGroupInteractions(CollisionGroup collisionGroup);
	~PhysicsManager();

	// Box2D
	b2World* box2dWorld;
	b2Body* worldStaticBody;

private:

	std::map<CollisionGroup, unsigned int> collisionGroupInteractions;

};

#endif
