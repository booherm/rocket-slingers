#ifndef POROPE_HPP
#define POROPE_HPP

#include "PhysicalObject.hpp"
#include "PoGuy.hpp"

class PoRope : public PhysicalObject {
public:

	PoRope(const std::string& objectId, GameState* gameState);
	void doPhysicalUpdate();
	void render();
	void gameEventCallback(const Event& eventObj);
	~PoRope();

private:

	struct RopeSegment {
		PhysicalMass* ropeMass;
		btHingeConstraint* linkConstraint;
	};

	bool attachedToStructure;
	unsigned int ropeMassesCount;
	float maxRopeLength;
	float maxRopeSegmentLength;
	std::vector<RopeSegment> ropeMasses;
	PoGuy* player = nullptr;

	void initShaders();
	void initGeometry();
	void destructRope();

};

#endif
