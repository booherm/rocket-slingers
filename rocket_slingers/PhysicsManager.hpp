#ifndef PHYSICSMANAGER_HPP
#define PHYSICSMANAGER_HPP

#include <Box2D/Box2D.h>
#include <map>

class PhysicsManager : public b2ContactListener {
public:

	enum CollisionCategory {
		NO_COLLISION     = 0,
		ROPE_SEGMENT     = 1,
		PLAYER           = 2,
		BOUNDARY         = 4,
		SWINGING_MASS    = 8,
		ATTACHMENT_POINT = 16,
		DRAGABLE_MASS    = 32
	};

	PhysicsManager();
	void setBox2dDebugRenderer(b2Draw* debugRenderer);
	void updatePhysics();
	unsigned int getCollisionMask(CollisionCategory collisionCategory);

	// b2ContactListener implementation
	void BeginContact(b2Contact* contact);
	void EndContact(b2Contact* contact);

	~PhysicsManager();

	// Box2D
	b2World* box2dWorld;
	b2Body* worldStaticBody;

private:

	std::map<CollisionCategory, unsigned int> collisionMasks;

};

#endif
