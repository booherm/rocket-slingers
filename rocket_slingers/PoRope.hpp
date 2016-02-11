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

	const unsigned int ropeMassCount = 20;
	const float springStiffnessConstant = 4500.0f;
	const float internalSpringFrictionConstant = 200.2f;
	const float airFrictionConstant = 3.02f;
	std::vector<RopeMass> ropeMasses;

	void initGeometry();
};

#endif
