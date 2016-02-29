#include "PhysicalObject.hpp"

PhysicalObject::PhysicalObject(const std::string& objectId, GameState* gameState) : RenderableObject(objectId) {
	this->objectId = objectId;
	this->gameState = gameState;
	shouldDoPhysicalUpdate = false;
}

PhysicalObject::~PhysicalObject() {}

void PhysicalObject::updatePhysicalState() {
	if (shouldDoPhysicalUpdate) {
		doPhysicalUpdate();
	}
}

void PhysicalObject::sdlInputEventCallback(const Event& eventObj) {}

void PhysicalObject::gameEventCallback(const Event& eventObj) {}

void PhysicalObject::doPhysicalUpdate() {}
