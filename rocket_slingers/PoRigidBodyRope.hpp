#ifndef PORIGIDBODYROPE_HPP
#define PORIGIDBODYROPE_HPP

#include "PhysicalObject.hpp"
#include "PoGuy.hpp"

class PoRigidBodyRope : public PhysicalObject {
public:

	PoRigidBodyRope(GameState* gameState);
	void doPhysicalUpdate();
	void doRenderUpdate();
	void gameEventCallback(const Event& eventObj);
	~PoRigidBodyRope();

private:

	struct RopeSegment {
		PhysicalMass* ropeMass;
		//btGeneric6DofSpring2Constraint* linkConstraint;
		//btGeneric6DofConstraint* linkConstraint;
		btHingeConstraint* linkConstraint;
	};

	float ropeSegmentLength;
	unsigned int ropeMassesCount;
	std::vector<RopeSegment> ropeMasses;
	PoGuy* player = nullptr;

	void initGeometry();
	void destructRope();
};

#endif
