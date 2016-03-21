#ifndef POROPEBOX2D_HPP
#define POROPEBOX2D_HPP

#include "PhysicalObject.hpp"
#include "PoGuyBox2d.hpp"

class PoRopeBox2d : public PhysicalObject {
public:

	PoRopeBox2d(const std::string& objectId, GameState* gameState);
	void doPhysicalUpdate();
	void render();
	void gameEventCallback(const Event& eventObj);
	~PoRopeBox2d();

private:

	struct RopeSegment {
		b2Body* body;
		b2RevoluteJoint* revJoint;
		//b2DistanceJoint* distJoint;
		b2RopeJoint* ropeJoint;
	};
	b2RevoluteJoint* anchorRevJoint;
	//b2RopeJoint* ropeMaxLengthJoint;

	bool attachedToStructure;
	unsigned int ropeSegmentsCount;
	float maxRopeLength;
	float maxRopeSegmentLength;
	float totalRopeMass;
	std::vector<RopeSegment> ropeSegments;
	PoGuyBox2d* player = nullptr;

	void initShaders();
	void initGeometry();
	void destructRope();

};

#endif
