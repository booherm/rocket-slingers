#ifndef WORLDRAYCASTCALLBACK_HPP
#define WORLDRAYCASTCALLBACK_HPP

#include <Box2D/Box2D.h>

class WorldRayCastCallback : public b2RayCastCallback {
public:
	float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction);

	b2Fixture* fixture = nullptr;
	b2Vec2 point;
	b2Vec2 normal;
	float32 fraction;

};

#endif
