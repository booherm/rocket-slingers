#include "PhysicalObject.hpp"

PhysicalObject::PhysicalObject(const std::string& objectId, GameState* gameState) : RenderableObject(objectId) {
	this->objectId = objectId;
	this->gameState = gameState;
	shouldDoPhysicalUpdate = false;
	bool ropeAttachable = false;
}

PhysicalObject::~PhysicalObject() {}

void PhysicalObject::updatePhysicalState() {
	if (shouldDoPhysicalUpdate) {
		doPhysicalUpdate();
	}
}

void PhysicalObject::sdlInputEventCallback(const Event& eventObj) {}

void PhysicalObject::gameEventCallback(const Event& eventObj) {}

void PhysicalObject::processContactBegin(PhysicalObject* contactingObject, b2Contact* contact) {}

void PhysicalObject::processContactEnd(PhysicalObject* contactingObject, b2Contact* contact) {}

void PhysicalObject::doPhysicalUpdate() {}

