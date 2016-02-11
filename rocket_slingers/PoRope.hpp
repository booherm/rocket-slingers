#ifndef POROPE_HPP
#define POROPE_HPP

#include "PhysicalObject.hpp"

class PoRope : public PhysicalObject {
public:
	PoRope(GameState* gameState);
	void updatePhysicalState();
	void inputEventCallback(InputEvent inputEvent);

private:

	struct RopeMass {
		float mass;
		float unstretchedLength;
		float stretchedLength;
		glm::vec3 force;
		glm::vec3 position;
		glm::vec3 velocity;
	};

	unsigned int ropeMassCount = 15;
	float ropeMassMass = 0.05f;
	float springStiffnessConstant = 10000.0f;
	float internalSpringFrictionConstant = 0.2f;
	float airFrictionConstant = 0.02f;
	float gravitationalConstant = 9.81f;
	const float maxAllowedChangeInTime = 0.002f;
	

	std::vector<RopeMass> ropeMasses;

	void initGeometry();
};

#endif
