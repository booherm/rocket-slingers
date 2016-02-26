#ifndef POPENDULUM_HPP
#define POPENDULUM_HPP

#include "PhysicalObject.hpp"

class PoPendulum : public PhysicalObject {
public:
	PoPendulum(GameState* gameState);
	void doRenderUpdate();
	~PoPendulum();

private:

	float sizeScaler = 5.0f;

	glm::vec3 initialPosition;
	void doPhysicalUpdate();
	void initGeometry();
	void initPhysics();
	
	PhysicalMass* hingePointMass;
	PhysicalMass* bobMass;
	btHingeConstraint* hingeConstraint;
};

#endif
