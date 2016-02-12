#include "PhysicalMass.hpp"

void PhysicalMass::connectMasses(PhysicalMass* massToConnectWith) {
	connectedPhysicalMasses.push_back(massToConnectWith);
	massToConnectWith->connectedPhysicalMasses.push_back(this);
}

void PhysicalMass::updatePhysics(float changeInTime) {
	if (mass != 0.0f) {
		velocity += ((force / mass) * changeInTime);
		worldPosition += (velocity * changeInTime);
	}
}

void PhysicalMass::resetForce() {
	this->force = glm::vec3();
}
