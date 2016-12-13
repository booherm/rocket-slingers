#ifndef WORLDRAYCASTER_HPP
#define WORLDRAYCASTER_HPP

#include <Box2D/Box2D.h>
#include "GameState.hpp"
#include "PhysicsManager.hpp"

class WorldRayCaster : public b2RayCastCallback {
public:
	
	WorldRayCaster(GameState* gameState, PhysicsManager::CollisionCategory collisionCategory);  // limit to these types
	float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction);
	b2Fixture* getFirstFixtureHit(const b2Vec2& from, const b2Vec2& to);
	b2Fixture* fixture;
	b2Vec2 point;
	b2Vec2 normal;
	float32 fraction;


private:
	GameState* gameState;
	bool firstFixtureDetectionType;
	PhysicsManager::CollisionCategory collisionCategory;
	unsigned int collisionMask;

};

#endif
