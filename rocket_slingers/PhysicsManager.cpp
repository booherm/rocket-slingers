#include "PhysicsManager.hpp"
#include <iostream>


PhysicsManager::PhysicsManager(){

	// init collision group interactions
	collisionGroupInteractions[NO_COLLISION] = NO_COLLISION;
	collisionGroupInteractions[ROPE_MASS] = SWINGING_MASS;
	collisionGroupInteractions[PLAYER] = PLAYER | BOUNDARY | SWINGING_MASS;
	collisionGroupInteractions[BOUNDARY] = PLAYER | SWINGING_MASS;
	collisionGroupInteractions[SWINGING_MASS] = PLAYER | BOUNDARY | ROPE_MASS;


	// Box2d
	box2dWorld = new b2World(b2Vec2(0.0f, -9.81f));

	// create a static body that joints can use as a refernce when wanting to constrain to the world
	b2BodyDef worldStaticBodyDef;
	worldStaticBody = box2dWorld->CreateBody(&worldStaticBodyDef);
}

unsigned int PhysicsManager::getCollisionGroupInteractions(CollisionGroup collisionGroup) {
	return collisionGroupInteractions[collisionGroup];
}

void PhysicsManager::setBox2dDebugRenderer(b2Draw* debugRenderer) {

	debugRenderer->SetFlags(
		0
		+ b2Draw::e_shapeBit	///< draw shapes
		+ b2Draw::e_jointBit	///< draw joint connections
		+ b2Draw::e_aabbBit	///< draw axis aligned bounding boxes
		+ b2Draw::e_pairBit      	///< draw broad-phase pairs
		+ b2Draw::e_centerOfMassBit
	);
	box2dWorld->SetDebugDraw(debugRenderer);

}

void PhysicsManager::updatePhysics() {
	box2dWorld->DrawDebugData();
	box2dWorld->Step(1 / 60.0f, 6, 10);
	//box2dWorld->Step(1 / 60.0f, 6, 20);
	//box2dWorld->Step(1 / 60.0f, 6, 1000);
}

PhysicsManager::~PhysicsManager() {
	
	box2dWorld->DestroyBody(worldStaticBody);
	delete box2dWorld;
}
