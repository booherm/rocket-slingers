#ifndef POPENDULUM2_HPP
#define POPENDULUM2_HPP

#include "PhysicalObject.hpp"

class PoPendulum2 : public PhysicalObject {
public:
	PoPendulum2(GameState* gameState);
	void doRenderUpdate();
	void sdlInputEventCallback(const Event& eventObj);
	~PoPendulum2();

private:

	float sizeScaler = 5.0f;

	glm::vec3 initialPosition;
	void doPhysicalUpdate();
	void initGeometry();
	void initPhysics();
	
	PhysicalMass* physicalMass;
	btHingeConstraint* hingeConstraint;
	btGeneric6DofSpring2Constraint* swingConstraint;
};

#endif
