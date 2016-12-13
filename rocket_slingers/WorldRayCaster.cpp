#include "WorldRayCaster.hpp"


WorldRayCaster::WorldRayCaster(GameState* gameState, PhysicsManager::CollisionCategory collisionCategory) {
	this->gameState = gameState;
	this->collisionCategory = collisionCategory;
	
	fixture = nullptr;
	collisionMask = gameState->physicsManager->getCollisionMask(collisionCategory);
}

b2Fixture* WorldRayCaster::getFirstFixtureHit(const b2Vec2& from, const b2Vec2& to) {
	firstFixtureDetectionType = true;
	gameState->physicsManager->box2dWorld->RayCast(this, from, to);

	return fixture;
}

float32 WorldRayCaster::ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction) {

	b2Filter filterData = fixture->GetFilterData();
	
	if (filterData.categoryBits & collisionMask) {
		this->fixture = fixture;
		this->point = point;
		this->normal = normal;
		this->fraction = fraction;

		if (firstFixtureDetectionType)
			return fraction;
	}

	return -1.0f;

}
