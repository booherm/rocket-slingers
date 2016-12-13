#ifndef POROPE_HPP
#define POROPE_HPP

#include "PhysicalObject.hpp"
#include "PoGuy.hpp"
#include "PoRopeTarget.hpp"

class PoRope : public PhysicalObject {
public:

	PoRope(const std::string& objectId, GameState* gameState);
	void doPhysicalUpdate();
	void render();
	void gameEventCallback(const Event& eventObj);
	~PoRope();

private:

	struct RopeSegment {
		b2Body* body;
		b2RevoluteJoint* revJoint;
	};

	bool attachedToStructure;
	unsigned int ropeSegmentsCount;
	float maxRopeLength;
	float maxRopeSegmentLength;
	float totalRopeMass;
	std::vector<RopeSegment> ropeSegments;
	PoGuy* player = nullptr;
	PoRopeTarget* ropeTarget;

	void initShaders();
	void initGeometry();
	void destructRope();

};

#endif
