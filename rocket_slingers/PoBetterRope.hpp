#ifndef POBETTERROPE_HPP
#define POBETTERROPE_HPP

#include "PhysicalObject.hpp"
#include "PoGuy.hpp"

class PoBetterRope : public PhysicalObject {
public:

	PoBetterRope(GameState* gameState);
	void doPhysicalUpdate();
	void doRenderUpdate();
	void gameEventCallback(const Event& eventObj);
	void sdlInputEventCallback(const Event& eventObj);
	~PoBetterRope();

private:

	float ropeSegmentLength;
	unsigned int ropeSegmentCount;
	PhysicalMassMultiBody* rope = nullptr;
	PoGuy* player = nullptr;
	glm::vec3 currentPlayerMassForce;

	void initGeometry();
};

#endif
