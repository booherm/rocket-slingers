#include "PhysicsManager.hpp"
#include <iostream>
#include "PhysicalObject.hpp"

PhysicsManager::PhysicsManager(){

	// init collision group interactions
	collisionMasks[NO_COLLISION] = NO_COLLISION;
	collisionMasks[ROPE_SEGMENT] = SWINGING_MASS | BOUNDARY | ATTACHMENT_POINT | DRAGABLE_MASS;
	collisionMasks[PLAYER] = PLAYER | BOUNDARY | SWINGING_MASS | DRAGABLE_MASS;
	collisionMasks[BOUNDARY] = PLAYER | SWINGING_MASS | ROPE_SEGMENT | DRAGABLE_MASS;
	collisionMasks[SWINGING_MASS] = PLAYER | BOUNDARY | ROPE_SEGMENT | DRAGABLE_MASS;
	collisionMasks[ATTACHMENT_POINT] = NO_COLLISION;
	collisionMasks[DRAGABLE_MASS] = PLAYER | BOUNDARY | SWINGING_MASS | DRAGABLE_MASS;

	// Box2d
	box2dWorld = new b2World(b2Vec2(0.0f, -9.81f));
	//box2dWorld = new b2World(b2Vec2(0.0f, 0.0f));
	box2dWorld->SetContactListener(this);

	// create a static body that joints can use as a refernce when wanting to constrain to the world
	b2BodyDef worldStaticBodyDef;
	worldStaticBody = box2dWorld->CreateBody(&worldStaticBodyDef);
}

unsigned int PhysicsManager::getCollisionMask(CollisionCategory collisionCategory) {
	return collisionMasks[collisionCategory];
}

void PhysicsManager::setBox2dDebugRenderer(b2Draw* debugRenderer) {

	debugRenderer->SetFlags(
		0
		+ b2Draw::e_shapeBit         // draw shapes
		+ b2Draw::e_jointBit         // draw joint connections
		+ b2Draw::e_aabbBit	         // draw axis aligned bounding boxes
		+ b2Draw::e_pairBit          // draw broad-phase pairs
		+ b2Draw::e_centerOfMassBit  // draw center of mass
	);
	box2dWorld->SetDebugDraw(debugRenderer);

}

void PhysicsManager::updatePhysics() {
	box2dWorld->Step(1 / 60.0f, 6, 10);
	box2dWorld->DrawDebugData();
}

void PhysicsManager::BeginContact(b2Contact* contact) {
	PhysicalObject* poA = (PhysicalObject*) contact->GetFixtureA()->GetUserData();
	PhysicalObject* poB = (PhysicalObject*) contact->GetFixtureB()->GetUserData();
	poA->processContactBegin(poB, contact);
	poB->processContactBegin(poA, contact);
}

void PhysicsManager::EndContact(b2Contact* contact) {
	PhysicalObject* poA = (PhysicalObject*) contact->GetFixtureA()->GetUserData();
	PhysicalObject* poB = (PhysicalObject*) contact->GetFixtureB()->GetUserData();
	poA->processContactEnd(poB, contact);
	poB->processContactEnd(poA, contact);
}

PhysicsManager::~PhysicsManager() {
	box2dWorld->DestroyBody(worldStaticBody);
	delete box2dWorld;
}
