#ifndef PHYSICALMASS_HPP
#define PHYSICALMASS_HPP

#include <glm.hpp>
#include <vector>

class PhysicalMass {
public:
	void connectMasses(PhysicalMass* massToConnectWith);
	void updatePhysics(float changeInTime);
	void resetForce();

	float mass;
	glm::vec3 force;
	glm::vec3 velocity;
	glm::vec3 worldPosition;
	std::vector<PhysicalMass*> connectedPhysicalMasses;
};

#endif
