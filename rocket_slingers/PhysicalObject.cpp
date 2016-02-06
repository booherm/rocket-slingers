#include "PhysicalObject.hpp"

PhysicalObject::PhysicalObject(GameState* gameState) {
	this->gameState = gameState;
}

void PhysicalObject::updatePhysicalState() {}
void PhysicalObject::render() {}
void PhysicalObject::inputEventCallback(InputEvent inputEvent) {}
PhysicalObject::~PhysicalObject() {}
void PhysicalObject::initGeometry() {}
void PhysicalObject::initShaders() {}
