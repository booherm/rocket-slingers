#ifndef POROPE_HPP
#define POROPE_HPP
/*
#include "PhysicalObject.hpp"
#include "PoSpring.hpp"

class PoRope : public PhysicalObject {
public:
	PoRope(GameState* gameState);
	void updatePhysicalState();
	void render();
	void inputEventCallback(InputEvent inputEvent);
	~PoRope();

private:

	struct RopeMass {
		float mass;
		float unstretchedLength;
		float stretchedLength;
		glm::vec3 force;
		glm::vec3 position;
		glm::vec3 velocity;
	};

	const unsigned int maxRopeCount = 100;
	const unsigned int ropeMassCount = 20;
	const float springStiffnessConstant = 4500.0f;
	const float internalSpringFrictionConstant = 200.2f;
	const float airFrictionConstant = 3.02f;
	std::vector<std::vector<RopeMass>> allRopeMassesContainer;
	unsigned int ropeCount = 0;
	void initGeometry();
	void initShaders();
	float springLength = 0.05f;
};
*/
#endif
