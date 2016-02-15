#ifndef POROPE_HPP
#define POROPE_HPP

#include "PhysicalObject.hpp"

class PoRope : public PhysicalObject {
public:
	PoRope(GameState* gameState);
	void doPhysicalUpdate();
	void doRenderUpdate();
	void gameEventCallback(const Event& eventObj);
	void sdlInputEventCallback(const Event& eventObj);

private:

	struct RopeSegmentLength {
		float unstretchedLength;
		float stretchedLength;
	};

	unsigned int ropeMassCount = 15;
	float ropeMassMass = 0.05f;
	float springStiffnessConstant = 10000.0f;
	float internalSpringFrictionConstant = 0.2f;
	float airFrictionConstant = 0.02f;
	float gravitationalConstant = 9.81f;

	std::vector<RopeSegmentLength> ropeSegmentLengths;
	void initGeometry();
};

#endif
